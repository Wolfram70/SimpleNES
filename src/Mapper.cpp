#include "../include/Mapper.h"

//MAPPER 000

bool Mapper_000::read_map_cpu(uint16_t addr, uint32_t &mapped_addr)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
    {
        mapped_addr = addr & (n_prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data)
{
    if(addr >= 0x8000 && addr <= 0xFFFF)
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