#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

#include "Mapper.h"

class Cartridge
{
public:
    Cartridge(const std::string &filepath);
    ~Cartridge();

public:
    Mapper::MIRROR mirror = Mapper::MIRROR::HORIZONTAL;

private:
    //memory
    std::vector<uint8_t> prg_rom; //program ROM
    std::vector<uint8_t> chr_rom; //character ROM

    //mapper
    std::shared_ptr<Mapper> mapper;

    uint8_t mapper_id = 0; //mapper ID
    uint16_t prg_banks = 0; //number of 16KB PRG ROM banks
    uint16_t chr_banks = 0; //number of 8KB CHR ROM banks
    uint32_t prg_ram_size = 0; //size of PRG RAM in 8KB units
    uint32_t chr_ram_size = 0; //size of CHR RAM in 8KB units

    bool valid = true;

public:
    //communication with the main bus (CPU bus)
    bool read_cpu(uint16_t addr, uint8_t &data);
    bool write_cpu(uint16_t addr, uint8_t data);

    //communication with the PPU bus
    bool read_ppu(uint16_t addr, uint8_t &data);
    bool write_ppu(uint16_t addr, uint8_t data);
    bool getValid() { return valid; }

    void reset() { if(mapper.get() != nullptr) mapper->reset(); }
};

#endif