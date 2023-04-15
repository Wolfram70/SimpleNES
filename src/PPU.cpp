#include "../include/Bus.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

PPU::PPU()
{
    srand(time(NULL));
    screen.create(256, 240);
    name_table[0].create(256, 240);
    name_table[1].create(256, 240);
    pattern_table[0].create(128, 128);
    pattern_table[1].create(128, 128);

    //initialize the palette table
    palette_table[0x00] = sf::Color(84, 84, 84);
	palette_table[0x01] = sf::Color(0, 30, 116);
	palette_table[0x02] = sf::Color(8, 16, 144);
	palette_table[0x03] = sf::Color(48, 0, 136);
	palette_table[0x04] = sf::Color(68, 0, 100);
	palette_table[0x05] = sf::Color(92, 0, 48);
	palette_table[0x06] = sf::Color(84, 4, 0);
	palette_table[0x07] = sf::Color(60, 24, 0);
	palette_table[0x08] = sf::Color(32, 42, 0);
	palette_table[0x09] = sf::Color(8, 58, 0);
	palette_table[0x0A] = sf::Color(0, 64, 0);
	palette_table[0x0B] = sf::Color(0, 60, 0);
	palette_table[0x0C] = sf::Color(0, 50, 60);
	palette_table[0x0D] = sf::Color(0, 0, 0);
	palette_table[0x0E] = sf::Color(0, 0, 0);
	palette_table[0x0F] = sf::Color(0, 0, 0);

	palette_table[0x10] = sf::Color(152, 150, 152);
	palette_table[0x11] = sf::Color(8, 76, 196);
	palette_table[0x12] = sf::Color(48, 50, 236);
	palette_table[0x13] = sf::Color(92, 30, 228);
	palette_table[0x14] = sf::Color(136, 20, 176);
	palette_table[0x15] = sf::Color(160, 20, 100);
	palette_table[0x16] = sf::Color(152, 34, 32);
	palette_table[0x17] = sf::Color(120, 60, 0);
	palette_table[0x18] = sf::Color(84, 90, 0);
	palette_table[0x19] = sf::Color(40, 114, 0);
	palette_table[0x1A] = sf::Color(8, 124, 0);
	palette_table[0x1B] = sf::Color(0, 118, 40);
	palette_table[0x1C] = sf::Color(0, 102, 120);
	palette_table[0x1D] = sf::Color(0, 0, 0);
	palette_table[0x1E] = sf::Color(0, 0, 0);
	palette_table[0x1F] = sf::Color(0, 0, 0);

	palette_table[0x20] = sf::Color(236, 238, 236);
	palette_table[0x21] = sf::Color(76, 154, 236);
	palette_table[0x22] = sf::Color(120, 124, 236);
	palette_table[0x23] = sf::Color(176, 98, 236);
	palette_table[0x24] = sf::Color(228, 84, 236);
	palette_table[0x25] = sf::Color(236, 88, 180);
	palette_table[0x26] = sf::Color(236, 106, 100);
	palette_table[0x27] = sf::Color(212, 136, 32);
	palette_table[0x28] = sf::Color(160, 170, 0);
	palette_table[0x29] = sf::Color(116, 196, 0);
	palette_table[0x2A] = sf::Color(76, 208, 32);
	palette_table[0x2B] = sf::Color(56, 204, 108);
	palette_table[0x2C] = sf::Color(56, 180, 204);
	palette_table[0x2D] = sf::Color(60, 60, 60);
	palette_table[0x2E] = sf::Color(0, 0, 0);
	palette_table[0x2F] = sf::Color(0, 0, 0);

	palette_table[0x30] = sf::Color(236, 238, 236);
	palette_table[0x31] = sf::Color(168, 204, 236);
	palette_table[0x32] = sf::Color(188, 188, 236);
	palette_table[0x33] = sf::Color(212, 178, 236);
	palette_table[0x34] = sf::Color(236, 174, 236);
	palette_table[0x35] = sf::Color(236, 174, 212);
	palette_table[0x36] = sf::Color(236, 180, 176);
	palette_table[0x37] = sf::Color(228, 196, 144);
	palette_table[0x38] = sf::Color(204, 210, 120);
	palette_table[0x39] = sf::Color(180, 222, 120);
	palette_table[0x3A] = sf::Color(168, 226, 144);
	palette_table[0x3B] = sf::Color(152, 226, 180);
	palette_table[0x3C] = sf::Color(160, 214, 228);
	palette_table[0x3D] = sf::Color(160, 162, 160);
	palette_table[0x3E] = sf::Color(0, 0, 0);
	palette_table[0x3F] = sf::Color(0, 0, 0);
}

PPU::~PPU()
{

}

//read and write functions for the PPU

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
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.vertical_blank = 0;
            address_latch = 0;
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
            data = ppu_data_buffer;
            ppu_data_buffer = read_ppu(ppu_address);
            if(ppu_address > 0x3F00) data = ppu_data_buffer;
            ppu_address++;
            break;
    }

    return data;
}

void PPU::write_cpu(uint16_t addr, uint8_t data)
{
    switch(addr)
    {
        case 0x0000: //control register
            control.reg = data;
            break;
        case 0x0001: //mask register
            mask.reg = data;
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
            if(address_latch == 0)
            {
                ppu_address = (ppu_address & 0x00FF) | (uint16_t(data) << 8);
                address_latch = 1;
            }
            else
            {
                ppu_address = (ppu_address & 0xFF00) | uint16_t(data);
                address_latch = 0;
            }
            break;
        case 0x0007: //PPU data register
            write_ppu(ppu_address, data);
            ppu_address++;
            break;
    }
}

uint8_t PPU::read_ppu(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;
    addr &= 0x3FFF; //mask the address to valid range

    if(cartridge->read_ppu(addr, data))
    {
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF) //pattern table
    {
        data = pattern[(addr & 0x1000) >> 12][addr & 0x0FFF]; //first dim chooses which 4KB by examining MSB
    }
    else if(addr >= 0x2000 && addr <= 0x3EFF) //nametable
    {

    }
    else if(addr >= 0x3F00 && addr <= 0x3FFF) //palette table
    {
        addr &= 0x001F;
        //mirroring
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;

        data = palette[addr];
    }

    return data;
}

void PPU::write_ppu(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF; //mask the address to valid range

    if(cartridge->write_ppu(addr, data))
    {

    }
    else if(addr >= 0x0000 && addr <= 0x1FFF) //pattern table
    {
        pattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if(addr >= 0x2000 && addr <= 0x3EFF) //nametable
    {

    }
    else if(addr >= 0x3F00 && addr <= 0x3FFF) //palette table
    {
        addr &= 0x001F;
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;

        palette[addr] = data;
    }
}


//helper functions for the PPU
void PPU::connect_cartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cartridge = cartridge;
}

void PPU::clock()
{
    if(scanline == -1 && cycle == 1)
    {
        status.vertical_blank = 0;
    }

    if(scanline == 241 && cycle == 1)
    {
        status.vertical_blank = 1;
        if(control.enable_nmi) nmi = true;
    }
    //if(cycle >= 0 && cycle < 256 && scanline >= 0 && scanline < 240) screen.setPixel(cycle, scanline, ((rand() % 2 == 0) ? palette_table[0x3F] : palette_table[0x30]));
    cycle++;
    if(cycle >= 341)
    {
        cycle = 0;
        scanline++;
        if(scanline >= 261)
        {
            scanline = -1;
            frame_complete = true;
        }
    }
}

sf::Color& PPU::getColorFromPaletteRam(uint8_t palette, uint8_t pixel)
{
    uint16_t address = 0x3F00 + (palette << 2) + pixel;
    uint8_t index = read_ppu(address);
    return palette_table[index & 0x3F];
}

sf::Image& PPU::getPatternTable(uint8_t i, uint8_t palette)
{
    for(uint16_t n_tiley = 0; n_tiley < 16; n_tiley++)
    {
        for(uint16_t n_tilex = 0; n_tilex < 16; n_tilex++)
        {
            uint16_t n_offset = n_tiley * 256 + n_tilex * 16;

            for(uint16_t row = 0; row < 8; row++)
            {
                uint8_t tile_lsb = read_ppu(i * 0x1000 + n_offset + row + 0x0000);
                uint8_t tile_msb = read_ppu(i * 0x1000 + n_offset + row + 0x0008);

                for(uint16_t col = 0; col < 8; col++)
                {
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
                    tile_lsb >>= 1;
                    tile_msb >>= 1;

                    pattern_table[i].setPixel(n_tilex * 8 + (7 - col), n_tiley * 8 + row, getColorFromPaletteRam(palette, pixel));
                }
            }
        }
    }
    return pattern_table[i];
}