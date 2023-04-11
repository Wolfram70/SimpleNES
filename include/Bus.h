#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include <array>
#include "CPU.h"

class Bus
{
public:
    Bus();
    ~Bus();

private: //devices on the bus
    CPU cpu; //the 6502 processor
    
    std::array<uint8_t, 64 * 1024> ram; //64KB of RAM

public: //read and write
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};
#endif