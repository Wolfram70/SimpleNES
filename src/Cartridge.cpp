#include "../include/Cartridge.h"
#include <fstream>
#include <iostream>

//iNES Header
struct iNESHeader
{
    char name[4]; //should be "NES" + EOF
    uint8_t prg_rom_chunks; //number of 16KB prg rom chunks
    uint8_t chr_rom_chunks; //number of 8KB chr rom chunks
    uint8_t flags_6; //mapper, mirroring, battery, trainer
    uint8_t flags_7; //mapper, VS/Playchoice, NES 2.0
    uint8_t prg_ram_size; //number of 8KB prg ram chunks
    uint8_t flags_9; //TV system, unused
    uint8_t flags_10; //TV system, unused
    uint8_t padding[5]; //unused padding
};

Cartridge::Cartridge(const std::string &filepath)
{
    iNESHeader header;
    //open the file in binary mode
    std::ifstream ifs;
    ifs.open(filepath, std::ios::binary);
    if(ifs.is_open())
    {
        ifs.read((char*)&header, sizeof(iNESHeader)); //read the header

        //next 512 bytes are the trainer (can be ignored)
        if(header.flags_6 & 0x04)
        {
            ifs.seekg(512, std::ios::cur);
        }

        //detect the mapper
        mapper_id = (header.flags_7 & 0xF0) | ((header.flags_6 & 0xF0) >> 4); //4 bits from flags_7 and 4 bits from flags_6

        uint8_t fileformat = 1; //for now
        if(fileformat == 0)
        {

        }
        else if(fileformat == 1)
        {
            //read the PRG ROM
            prg_banks = header.prg_rom_chunks;
            prg_rom.resize(prg_banks * 16384);
            ifs.read((char*)prg_rom.data(), prg_rom.size());

            //read the CHR ROM
            chr_banks = header.chr_rom_chunks;
            chr_rom.resize(chr_banks * 8192);
            ifs.read((char*)chr_rom.data(), chr_rom.size());
        }
        else if(fileformat == 2)
        {

        }

        //load the mapper
        switch(mapper_id)
        {
            case 0:
                mapper = std::make_shared<Mapper_000>(prg_banks, chr_banks);
                break;
            default:
                std::cout << "Mapper not implemented!" << std::endl;
                break;
        }

        ifs.close();
    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::read_cpu(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr;
    if(mapper->read_map_cpu(addr, mapped_addr))
    {
        data = prg_rom[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::write_cpu(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr;
    if(mapper->write_map_cpu(addr, mapped_addr))
    {
        prg_rom[mapped_addr] = data;
        return true;
    }
    return false;
}

bool Cartridge::read_ppu(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr;
    if(mapper->read_map_ppu(addr, mapped_addr))
    {
        data = chr_rom[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::write_ppu(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr;
    if(mapper->write_map_ppu(addr, mapped_addr))
    {
        chr_rom[mapped_addr] = data;
        return true;
    }
    return false;
}
