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
    enum MIRROR
    {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI
    } mirror = HORIZONTAL;

private:
    //memory
    std::vector<uint8_t> prg_rom; //program ROM
    std::vector<uint8_t> chr_rom; //character ROM

    //mapper
    std::shared_ptr<Mapper> mapper;

    uint8_t mapper_id = 0; //mapper ID
    uint8_t prg_banks = 0; //number of 16KB PRG ROM banks
    uint8_t chr_banks = 0; //number of 8KB CHR ROM banks

    bool valid = true;

public:
    //communication with the main bus (CPU bus)
    bool read_cpu(uint16_t addr, uint8_t &data);
    bool write_cpu(uint16_t addr, uint8_t data);

    //communication with the PPU bus
    bool read_ppu(uint16_t addr, uint8_t &data);
    bool write_ppu(uint16_t addr, uint8_t data);
    bool getValid() { return valid; }
};

#endif