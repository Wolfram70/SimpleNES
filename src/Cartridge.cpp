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
    char padding[5]; //unused padding
};

Cartridge::Cartridge(const std::string &filepath)
{
    iNESHeader header;
    //open the file in binary mode
    std::ifstream ifs;
    ifs.open(filepath, std::ios::binary);

    std::cout << "Loading ROM: " << filepath << std::endl;

    if(ifs.is_open())
    {
        std::cout << "File opened successfully!" << std::endl;

        std::cout << "Reading header..." << std::endl;
        ifs.read((char*)&header, sizeof(iNESHeader)); //read the header

        //next 512 bytes are the trainer (can be ignored)
        std::cout << "Reading trainer..." << std::endl;
        if(header.flags_6 & 0x04)
        {
            ifs.seekg(512, std::ios_base::cur);
        }

        //detect the mapper
        std::cout << "Detecting mapper..." << std::endl;
        mapper_id = (header.flags_7 & 0xF0) | ((header.flags_6 & 0xF0) >> 4); //4 bits from flags_7 and 4 bits from flags_6
        std::cout << "Got Mapper ID: " << (int)mapper_id << std::endl;

        //detect the mirroring
        std::cout << "Detecting mirroring..." << std::endl;
        if(header.flags_6 & 0x01)
        {
            mirror = Mapper::MIRROR::VERTICAL;
            std::cout << "Got Mirroring: Vertical" << std::endl;
        }
        else
        {
            mirror = Mapper::MIRROR::HORIZONTAL;
            std::cout << "Got Mirroring: Horizontal" << std::endl;
        }

        uint8_t fileformat = 1; //for now
        if((header.flags_7 & 0x0C) == 0x08) fileformat = 2;

        if(fileformat == 0)
        {

        }
        else if(fileformat == 1)
        {
            //read the PRG ROM
            std::cout << "Reading PRG ROM..." << std::endl;
            prg_banks = header.prg_rom_chunks;
            prg_rom.resize(prg_banks * 16384);
            ifs.read((char*)prg_rom.data(), prg_rom.size());

            //read the CHR ROM
            std::cout << "Reading CHR ROM..." << std::endl;
            chr_banks = header.chr_rom_chunks;
            if(chr_banks == 0)
            {
                chr_rom.resize(8192);
            }
            else
            {
                chr_rom.resize(chr_banks * 8192);
            }
            ifs.read((char*)chr_rom.data(), chr_rom.size());
        }
        else if(fileformat == 2)
        {
            prg_banks = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
			prg_rom.resize(prg_banks * 16384);
			ifs.read((char*)prg_rom.data(), prg_rom.size());

			chr_banks = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;
			chr_rom.resize(chr_banks * 8192);
			ifs.read((char*)chr_rom.data(), chr_rom.size());
        }

        //load the mapper
        switch(mapper_id)
        {
            case 0:
                mapper = std::make_shared<Mapper_000>(prg_banks, chr_banks);
                mapper->set_mirror(mirror);
                std::cout << "PRG_BANKS: " << (uint16_t)prg_banks << "   CHR_BANKS: " << (uint16_t)chr_banks << std::endl;
                break;
            case 1:
                mapper = std::make_shared<Mapper_001>(prg_banks, chr_banks);
                mapper->set_mirror(mirror);
                std::cout << "PRG_BANKS: " << (uint16_t)prg_banks << "   CHR_BANKS: " << (uint16_t)chr_banks << std::endl;
                break;
            default:
                std::cout << "Mapper not implemented!" << std::endl;
                valid = false;
                break;
        }

        ifs.close();
    }
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        valid = false;
    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::read_cpu(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr = 0;
    if(mapper->read_map_cpu(addr, mapped_addr, data))
    {
        if(mapped_addr == 0xFFFFFFFF) return true;
        data = prg_rom[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::write_cpu(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if(mapper->write_map_cpu(addr, mapped_addr, data))
    {
        if(mapped_addr == 0xFFFFFFFF) return true;
        prg_rom[mapped_addr] = data;
        return true;
    }
    mirror = mapper->get_mirror();
    return false;
}

bool Cartridge::read_ppu(uint16_t addr, uint8_t &data)
{
    uint32_t mapped_addr = 0;
    if(mapper->read_map_ppu(addr, mapped_addr))
    {
        if(mapped_addr >= chr_rom.size()) return false;
        data = chr_rom[mapped_addr];
        return true;
    }
    return false;
}

bool Cartridge::write_ppu(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if(mapper->write_map_ppu(addr, mapped_addr))
    {
        if(mapped_addr >= chr_rom.size()) return false;
        chr_rom[mapped_addr] = data;
        return true;
    }
    return false;
}
