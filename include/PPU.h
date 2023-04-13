#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <memory>

class Cartridge;

class PPU
{
public:
    PPU();
    ~PPU();

private:
    std::shared_ptr<Cartridge> cartridge; //the cartridge inserted in the console

private:
    uint8_t nametable[2][1024]; //2 nametables, each 1KB
    uint8_t palette[32]; //32 bytes of palette memory

public:
    //communication with the main bus (CPU bus)
    uint8_t read_cpu(uint16_t addr, bool bReadOnly = false);
    void write_cpu(uint16_t addr, uint8_t data);

    //communication with the PPU bus
    uint8_t read_ppu(uint16_t addr, bool bReadOnly = false);
    void write_ppu(uint16_t addr, uint8_t data);

public:
    //interface
    void connect_cartridge(const std::shared_ptr<Cartridge> &cartridge);
    void clock();
};

#endif