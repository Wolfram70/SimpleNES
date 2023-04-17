#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>
#include <memory>
#include "CPU.h"
#include "PPU.h"
#include "Cartridge.h"

class Bus
{
public:
    Bus();
    ~Bus();

public: //devices on the bus
    CPU cpu; //the 6502 processor
    PPU ppu; //the 2C02 PPU
    
    std::array<uint8_t, 2048> cpu_ram; //2KB of RAM
    std::shared_ptr<Cartridge> cartridge; //the cartridge inserted in the console

    //controller
    uint8_t controller[2];

    uint32_t nSystemClockCounter = 0;

private:
    //DMA
    uint8_t dma_page = 0x00;
    uint8_t dma_addr = 0x00;
    uint8_t dma_data = 0x00;

    bool dma_transfer = false;
    bool dma_dummy = true;

public:
    //controller state
    uint8_t controller_state[2];

public: //read and write
    void write_cpu(uint16_t addr, uint8_t data);
    uint8_t read_cpu(uint16_t addr, bool bReadOnly = false);

public:
    //interface
    void insert_cartridge(const std::shared_ptr<Cartridge> &cartridge);
    void reset();
    void clock();
};
#endif