#include "../include/PPU.h"

PPU::PPU()
{

}

PPU::~PPU()
{

}

uint8_t PPU::read_cpu(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    switch(addr)
    {
        case 0x0000: //control register
            break;
        case 0x0001: //mask register
            break;
        case 0x0002: //status register
            break;
        case 0x0003: //OAM address register
            break;
        case 0x0004: //OAM data register
            break;
        case 0x0005: //scroll register
            break;
        case 0x0006: //PPU address register
            break;
        case 0x0007: //PPU data register
            break;
    }

    return data;
}

void PPU::write_cpu(uint16_t addr, uint8_t data)
{
    switch(addr)
    {
        case 0x0000: //control register
            break;
        case 0x0001: //mask register
            break;
        case 0x0002: //status register
            break;
        case 0x0003: //OAM address register
            break;
        case 0x0004: //OAM data register
            break;
        case 0x0005: //scroll register
            break;
        case 0x0006: //PPU address register
            break;
        case 0x0007: //PPU data register
            break;
    }
}

uint8_t PPU::read_ppu(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;
    addr &= 0x3FFF; //mask the address to valid range

    return data;
}

void PPU::write_ppu(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF; //mask the address to valid range
}

void PPU::connect_cartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cartridge = cartridge;
}

void PPU::clock()
{
}
