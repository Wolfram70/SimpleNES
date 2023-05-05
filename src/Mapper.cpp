#include "../include/Mapper.h"

//MAPPER 000

bool Mapper_000::read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data)
{
    if(addr >= 0x6000 && addr <= 0x7FFF)
    {
        data = PRG_RAM[addr & 0x1FFF];
        mapped_addr = 0xFFFFFFFF;
        return true;
    }
    else if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (n_prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if(addr >= 0x6000 && addr <= 0x7FFF)
    {
        PRG_RAM[addr & 0x1FFF] = data;
        mapped_addr = 0xFFFFFFFF;
        return true;
    }
    else if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (n_prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::read_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_000::write_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        if(n_chr_banks == 0)
        {
            mapped_addr = addr;
            return true;
        }
    }
    
    return false;
}

//MAPPER 001

//TODO: consecutive cycle writes ignore
//TODO: Add support to SXROM, SOROM, SUROM (the ones with more than 8KB PRG RAM and 8KB CHR RAM) (only with NES2 headers)
//NOTE: Only a handful of games seem to be SOROM, and SXROM and SUROM are japan-only, so not urgent, SNROM seems to be working (detailed tests needed)

bool Mapper_001::read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data)
{
    if(addr >= 0x6000 && addr <= 0x7FFF)
    {
        data = PRG_RAM[addr & 0x1FFF];
        mapped_addr = 0xFFFFFFFF;
        return true;
    }
    else if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        uint8_t mode = (CR & 0x0C) >> 2;
        uint8_t index;
        uint32_t bank_256k = 0;
        //TODO: add support for 512k prg rom, chr0 and chr1 msb hold the bank select
        switch(mode)
        {
            case 0:
            case 1:
                index = (PRG & 0x0F) >> 1;
                mapped_addr = uint32_t(index << 15) | uint32_t(addr & 0x7FFF);
                break;
            case 2:
                if(addr >= 0x8000 && addr <= 0xBFFF)
                {
                    mapped_addr = addr & 0x3FFF;
                }
                else if(addr >= 0xC000 && addr <= 0xFFFF)
                {
                    index = PRG & 0x0F;
                    mapped_addr = uint32_t(index << 14) | uint32_t(addr & 0x3FFF);
                }
                break;

            case 3:
                if(addr >= 0x8000 && addr <= 0xBFFF)
                {
                    index = PRG & 0x0F;
                    mapped_addr = uint32_t(index << 14) | uint32_t(addr & 0x3FFF);
                }
                else if(addr >= 0xC000 && addr <= 0xFFFF)
                {
                    index = n_prg_banks - 1;
                    mapped_addr = uint32_t(index << 14) | uint32_t(addr & 0x3FFF);
                }
                break;

            default:
                break;
        }
        return true;
    }
    return false;
}

bool Mapper_001::write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if(addr >= 0x6000 && addr <= 0x7FFF)
    {
        PRG_RAM[addr & 0x1FFF] = data;
        mapped_addr = 0xFFFFFFFF;
        return true;
    }
    else if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        if(data & 0x80)
        {
            //RESET
            SR = 0x10;
            CR = CR | 0x0C;
        }
        else if(SR & 0x01)
        {
            SR = SR >> 1;
            SR = SR | ((data & 0x01) << 4);
            
            if(addr >= 0x8000 && addr <= 0x9FFF)
            {
                CR = SR & 0x1F;
                uint8_t index = CR & 0x03;
                switch (index)
                {
                    case 0:
                        mirror = MIRROR::ONESCREEN_LO;
                        break;
                    case 1:
                        mirror = MIRROR::ONESCREEN_HI;
                        break;
                    case 2:
                        mirror = MIRROR::VERTICAL;
                        break;
                    case 3:
                        mirror = MIRROR::HORIZONTAL;
                        break;
                    default:
                        break;
                }
                SR = 0x10;
            }
            else if(addr >= 0xA000 && addr <= 0xBFFF)
            {
                CHR0 = SR;
                SR = 0x10;
            }
            else if(addr >= 0xC000 && addr <= 0xDFFF)
            {
                CHR1 = SR;
                SR = 0x10;
            }
            else if(addr >= 0xE000 && addr <= 0xFFFF)
            {
                PRG = SR;
                SR = 0x10;
            }
        }
        else
        {
            SR = SR >> 1;
            SR = SR | ((data & 0x01) << 4);
        }
    }
    return false;
}

bool Mapper_001::read_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        if(n_chr_banks == 0)
        {
            mapped_addr = addr;
            return true;
        }
        else
        {
            uint8_t mode = (CR & 0x10) >> 4;
            uint8_t index;
            switch (mode)
            {
            case 0:
                index = CHR0 >> 1;
                mapped_addr = uint32_t(index << 13) | addr;
                break;

            case 1:
                if(addr >= 0x0000 && addr <= 0x0FFF)
                {
                    index = CHR0;
                    mapped_addr = uint32_t(index << 12) | (addr & 0x0FFF);
                }
                else if(addr >= 0x1000 && addr <= 0x1FFF)
                {
                    index = CHR1;
                    mapped_addr = uint32_t(index << 12) | (addr & 0x0FFF);
                }
                break;
            }
            return true;
        }
    }
    return false;
}

bool Mapper_001::write_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        if(n_chr_banks == 0)
        {
            mapped_addr = addr;
            return true;
        }
        return true;
    }
    return false;
}

void Mapper_001::reset()
{
    SR = 0x10;
    CR = 0x0C;
    PRG = 0;
    CHR0 = 0;
    CHR1 = 0;
}

//MAPPER 002

bool Mapper_002::read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data)
{
    if(addr >= 0x8000 && addr <= 0xBFFF)
    {
        mapped_addr = uint32_t(PRG << 14) | uint32_t(addr & 0x3FFF);
        return true;
    }
    else if(addr >= 0xC000 && addr <= 0xFFFF)
    {
        mapped_addr = uint32_t((n_prg_banks - 1) << 14) | uint32_t(addr & 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_002::write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        PRG = data;
        return true;
    }
    return false;
}

bool Mapper_002::read_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_002::write_map_ppu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x0000 && addr <= 0x1FFF)
    {
        if(n_chr_banks == 0)
        {
            mapped_addr = addr;
            return true;
        }
    }
    return false;
}

void Mapper_002::reset()
{
    PRG = 0x00;
}
