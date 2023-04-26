#include "../include/Bus.h"

#include <iostream>
#include <cstring>
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
            data = pOAM[oam_address];
            break;
        case 0x0005: //scroll register
            break;
        case 0x0006: //PPU address register
            break;
        case 0x0007: //PPU data register
            data = ppu_data_buffer;
            ppu_data_buffer = read_ppu(vram_addr.reg/*& 0xEFFF*/); //ADDED THIS TO PASS VRAM_ACCESS.NES TEST (makes SMB not run??)
            if(vram_addr.reg > 0x3F00) data = ppu_data_buffer;
            vram_addr.reg = vram_addr.reg + (control.increment_mode ? 32 : 1);
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
            tram_addr.nametable_x = control.nametable_x;
            tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001: //mask register
            mask.reg = data;
            break;
        case 0x0002: //status register
            break;
        case 0x0003: //OAM address register
            oam_address = data;
            break;
        case 0x0004: //OAM data register
            pOAM[oam_address] = data;
            break;
        case 0x0005: //scroll register
            if(address_latch == 0)
            {
                fine_x = data & 0x07;
                tram_addr.coarse_x = data >> 3;
                address_latch = 1;
            }
            else
            {
                tram_addr.fine_y = data & 0x07;
                tram_addr.coarse_y = data >> 3;
                address_latch = 0;
            }
            break;
        case 0x0006: //PPU address register
            if(address_latch == 0)
            {
                tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
                address_latch = 1;
            }
            else
            {
                tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
                vram_addr = tram_addr;
                address_latch = 0;
            }
            break;
        case 0x0007: //PPU data register
            write_ppu(vram_addr.reg, data);
            vram_addr.reg += (control.increment_mode ? 32 : 1);
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
        addr &= 0x0FFF; //mask to valid range
        if(cartridge->mirror == Mapper::MIRROR::VERTICAL)
        {
            //if vertical mirroring
            if(addr >= 0x0000 && addr <= 0x03FF) data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x0400 && addr <= 0x07FF) data = nametable[1][addr & 0x03FF];
            else if(addr >= 0x0800 && addr <= 0x0BFF) data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x0C00 && addr <= 0x0FFF) data = nametable[1][addr & 0x03FF];
        }
        else if(cartridge->mirror == Mapper::MIRROR::HORIZONTAL)
        {
            //if horizontal mirroring
            if(addr >= 0x0000 && addr <= 0x03FF) data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x0400 && addr <= 0x07FF) data = nametable[0][addr & 0x03FF];
            else if(addr >= 0x0800 && addr <= 0x0BFF) data = nametable[1][addr & 0x03FF];
            else if(addr >= 0x0C00 && addr <= 0x0FFF) data = nametable[1][addr & 0x03FF];
        }
    }
    else if(addr >= 0x3F00 && addr <= 0x3FFF) //palette table
    {
        addr &= 0x001F;
        //mirroring
        if(addr == 0x0010) addr = 0x0000;
        if(addr == 0x0014) addr = 0x0004;
        if(addr == 0x0018) addr = 0x0008;
        if(addr == 0x001C) addr = 0x000C;

        data = palette[addr] & (mask.grayscale ? 0x30 : 0x3F);
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
        addr &= 0x0FFF; //mask to valid range
        if(cartridge->mirror == Mapper::MIRROR::VERTICAL)
        {
            //if vertical mirroring
            if(addr >= 0x0000 && addr <= 0x03FF) nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0x0400 && addr <= 0x07FF) nametable[1][addr & 0x03FF] = data;
            else if(addr >= 0x0800 && addr <= 0x0BFF) nametable[0][addr & 0x03FF] = data;
            else if(addr >= 0x0C00 && addr <= 0x0FFF) nametable[1][addr & 0x03FF] = data;
        }
        else if(cartridge->mirror == Mapper::MIRROR::HORIZONTAL)
        {
            //if horizontal mirroring
            if(addr >= 0x000 && addr <= 0x3FF) nametable[0][addr & 0x3FF] = data;
            else if(addr >= 0x400 && addr <= 0x7FF) nametable[0][addr & 0x3FF] = data;
            else if(addr >= 0x800 && addr <= 0xBFF) nametable[1][addr & 0x3FF] = data;
            else if(addr >= 0xC00 && addr <= 0xFFF) nametable[1][addr & 0x3FF] = data;
        }
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

void PPU::reset()
{
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
    status.vertical_blank = 0;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void PPU::clock()
{  
    auto IncrementScrollX = [&]()
    {
        //coarse X used as each cycle loads a new tile
        if(mask.render_background || mask.render_sprites)
        {
            if(vram_addr.coarse_x == 31)
            {
                vram_addr.coarse_x = 0;
                vram_addr.nametable_x = ~vram_addr.nametable_x;
            }
            else
            {
                vram_addr.coarse_x++;
            }
        }
    };

    auto IncrementScrollY = [&]()
    {
        //fine Y used as each scanline updates a new pixel
        if(mask.render_background || mask.render_sprites)
        {
            if(vram_addr.fine_y < 7)
            {
                vram_addr.fine_y++;
            }
            else
            {
                vram_addr.fine_y = 0;
                if(vram_addr.coarse_y == 29)
                {
                    vram_addr.coarse_y = 0;
                    vram_addr.nametable_y = ~vram_addr.nametable_y;
                }
                else if(vram_addr.coarse_y == 31)
                {
                    vram_addr.coarse_y = 0;
                }
                else
                {
                    vram_addr.coarse_y++;
                }
            }
        }
    };

    auto TransferAddressX = [&]()
    {
        if(mask.render_background || mask.render_sprites)
        {
            vram_addr.nametable_x = tram_addr.nametable_x;
            vram_addr.coarse_x = tram_addr.coarse_x;
        }
    };

    auto TransferAddressY = [&]()
    {
        if(mask.render_background || mask.render_sprites)
        {
            vram_addr.fine_y = tram_addr.fine_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
            vram_addr.coarse_y = tram_addr.coarse_y;
        }
    };

    auto LoadBackgroundShifters = [&]()
    {
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb; //load the next tile LSB into the lower byte of the shifter
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb; //load the next tile MSB into the lower byte of the shifter
        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00); //load the next tile attribute into the lower byte of the shifter (inflate to 16 bits)
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00); //load the next tile attribute into the higher byte of the shifter (inflate to 16 bits)
    };

    auto UpdateShifters = [&]()
    {
        if(mask.render_background)
        {
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;
            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }

        if(mask.render_sprites && cycle >= 1 && cycle < 258)
        {
            for(int i = 0; i < sprite_count; i++)
            {
                if(sprite_scanline[i].x > 0)
                {
                    sprite_scanline[i].x--;
                }
                else
                {
                    sprite_shifter_pattern_lo[i] <<= 1;
                    sprite_shifter_pattern_hi[i] <<= 1;
                }
            }
        }
    };

    if(scanline >= -1 && scanline < 240)
    {
        if (scanline == 0 && cycle == 0)
		{
			// "Odd Frame" cycle skip
			cycle = 1;
            // LoadBackgroundShifters();
            // bg_next_tile_id = read_ppu(0x2000 | (vram_addr.reg & 0x0FFF));
		}

        if(scanline == -1 && cycle == 1)
        {
            status.vertical_blank = 0;
            status.sprite_zero_hit = 0;
            status.sprite_overflow = 0;

            for(int i = 0; i < 8; i++)
            {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }
        }

        if((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
        {
            UpdateShifters();
            switch((cycle - 1) % 8)
            {
                case 0:
                    LoadBackgroundShifters();
                    bg_next_tile_id = read_ppu(0x2000 | (vram_addr.reg & 0x0FFF));
                    break;
                case 2:
                    bg_next_tile_attrib = read_ppu(0x23C0 | (vram_addr.nametable_y << 11) | (vram_addr.nametable_x << 10) | ((vram_addr.coarse_y >> 2) << 3) | (vram_addr.coarse_x >> 2));
                    if(vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
                    if(vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
                    bg_next_tile_attrib &= 0x03;
                    break;
                case 4:
                    bg_next_tile_lsb = read_ppu((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram_addr.fine_y) + 0);
                    break;
                case 6:
                    bg_next_tile_msb = read_ppu((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vram_addr.fine_y) + 8);
                    break;
                case 7:
                    IncrementScrollX();
                    break;
            }
        }

        if(cycle == 256)
        {
            IncrementScrollY();
        }

        if(cycle == 257)
        {
            //reset x scroll
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if(cycle == 338 || cycle == 340)
        {
            bg_next_tile_id = read_ppu(0x2000 | (vram_addr.reg & 0x0FFF));
        }

        //Foreground rendering
        if(cycle == 257 && scanline >= 0)
        {
            std::memset(sprite_scanline, 0xFF, 8 * sizeof(Sprite));
            sprite_count = 0;

            uint8_t nOAMEntry = 0;
            sprite_zero_hit_possible = false;
            while(nOAMEntry < 64 && sprite_count < 9)
            {
                int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);
                if(diff >= 0 && diff < (control.sprite_size ? 16 : 8))
                {
                    if(sprite_count < 8)
                    {
                        if(nOAMEntry == 0)
                        {
                            sprite_zero_hit_possible = true;
                        }
                        memcpy(&sprite_scanline[sprite_count], &OAM[nOAMEntry], sizeof(Sprite));
                        sprite_count++;
                    }
                }
                nOAMEntry++;
                status.sprite_overflow = (sprite_count > 8);
            }
        }

        if(cycle == 340)
        {
            for(uint8_t i = 0; i < sprite_count; i++)
            {
                uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
                uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

                if(!control.sprite_size)
                {
                    //8x8 sprites
                    if(!(sprite_scanline[i].attributes & 0x80))
                    {
                        //Normal
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12) | (sprite_scanline[i].id << 4) | (scanline - sprite_scanline[i].y);
                    }
                    else
                    {
                        //Flipped
                        sprite_pattern_addr_lo = (control.pattern_sprite << 12) | (sprite_scanline[i].id << 4) | (7 - (scanline - sprite_scanline[i].y));
                    }
                }
                else
                {
                    //8x16 sprites
                    if(!(sprite_scanline[i].attributes & 0x80))
                    {
                        //Normal
                        if(scanline - sprite_scanline[i].y < 8)
                        {
                            sprite_pattern_addr_lo = ((sprite_scanline[i].id & 0x01) << 12) | ((sprite_scanline[i].id & 0xFE) << 4) | ((scanline - sprite_scanline[i].y) & 0x07);
                        }
                        else
                        {
                            sprite_pattern_addr_lo = ((sprite_scanline[i].id & 0x01) << 12) | (((sprite_scanline[i].id & 0xFE) + 1) << 4) | ((scanline - sprite_scanline[i].y) & 0x07);
                        }
                    }
                    else
                    {
                        //Flipped
                        if(scanline - sprite_scanline[i].y < 8)
                        {
                            sprite_pattern_addr_lo = ((sprite_scanline[i].id & 0x01) << 12) | ((sprite_scanline[i].id & 0xFE) << 4) | (7 - ((scanline - sprite_scanline[i].y) & 0x07));
                        }
                        else
                        {
                            sprite_pattern_addr_lo = ((sprite_scanline[i].id & 0x01) << 12) | (((sprite_scanline[i].id & 0xFE) + 1) << 4) | (7 - ((scanline - sprite_scanline[i].y) & 0x07));
                        }
                    }
                }

                sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
                sprite_pattern_bits_lo = read_ppu(sprite_pattern_addr_lo);
                sprite_pattern_bits_hi = read_ppu(sprite_pattern_addr_hi);

                if(sprite_scanline[i].attributes & 0x40)
                {
                    //Flip sprite horizontally
                    sprite_pattern_bits_lo = ((sprite_pattern_bits_lo & 0x55) << 1) | ((sprite_pattern_bits_lo & 0xAA) >> 1);
                    sprite_pattern_bits_lo = ((sprite_pattern_bits_lo & 0x33) << 2) | ((sprite_pattern_bits_lo & 0xCC) >> 2);
                    sprite_pattern_bits_lo = ((sprite_pattern_bits_lo & 0x0F) << 4) | ((sprite_pattern_bits_lo & 0xF0) >> 4);

                    sprite_pattern_bits_hi = ((sprite_pattern_bits_hi & 0x55) << 1) | ((sprite_pattern_bits_hi & 0xAA) >> 1);
                    sprite_pattern_bits_hi = ((sprite_pattern_bits_hi & 0x33) << 2) | ((sprite_pattern_bits_hi & 0xCC) >> 2);
                    sprite_pattern_bits_hi = ((sprite_pattern_bits_hi & 0x0F) << 4) | ((sprite_pattern_bits_hi & 0xF0) >> 4);
                }

                sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
                sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
            }
        }

        if(scanline == -1 && cycle >= 280 && cycle < 305)
        {
            //reset y scroll
            TransferAddressY();
        }
    }

    if (scanline == 240)
    {
        //nothing
    }

    if(scanline >= 241 && scanline < 261)
    {
        if(scanline == 241 && cycle == 1)
        {
            status.vertical_blank = 1;
            if(control.enable_nmi) nmi = true;
        }
    }

    //Rendering
    uint8_t bg_pixel = 0x00;
    uint8_t bg_palette = 0x00;


    // uint16_t bit_mux = 0x8000 >> fine_x;
    // uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
    // uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
    // bg_pixel = (p1_pixel << 1) | p0_pixel;

    // uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
    // uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
    // bg_palette = (bg_pal1 << 1) | bg_pal0;
    // if(cycle >= 0 && cycle < 257 && scanline >= 0 && scanline < 240) screen.setPixel(cycle - 1, scanline, getColorFromPaletteRam(bg_palette, bg_pixel));

    if(mask.render_background)
    {
        uint16_t bit_mux = 0x8000 >> fine_x;
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
        bg_pixel = (p1_pixel << 1) | p0_pixel;

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    uint8_t fg_pixel = 0x00;
    uint8_t fg_palette = 0x00;
    uint8_t fg_priority = 0x00;

    if(mask.render_sprites)
    {
        sprite_zero_being_rendered = false;
        for(uint8_t i = 0; i < sprite_count; i++)
        {
            if(sprite_scanline[i].x == 0)
            {
                uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
                uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
                fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

                fg_palette = (sprite_scanline[i].attributes & 0x03) + 0x04;
                fg_priority = (sprite_scanline[i].attributes & 0x20) == 0;

                if(fg_pixel != 0x00)
                {
                    if(i == 0) sprite_zero_being_rendered = 1;
                    break;
                }
            }
        }
    }

    uint8_t pixel = 0x00;
    uint8_t palette = 0x00;

    if(bg_pixel == 0 && fg_pixel == 0)
    {
        pixel = 0x00;
        palette = 0x00;
    }
    else if(bg_pixel == 0 && fg_pixel > 0)
    {
        pixel = fg_pixel;
        palette = fg_palette;
    }
    else if(bg_pixel > 0 && fg_pixel == 0)
    {
        pixel = bg_pixel;
        palette = bg_palette;
    }
    else if(bg_pixel > 0 && fg_pixel > 0)
    {
        if(fg_priority)
        {
            pixel = fg_pixel;
            palette = fg_palette;
        }
        else
        {
            pixel = bg_pixel;
            palette = bg_palette;
        }

        if(sprite_zero_hit_possible && sprite_zero_being_rendered)
        {
            if(mask.render_background && mask.render_sprites)
            {
                if(~(mask.render_background_left | mask.render_sprites_left))
                {
                    if(cycle >= 9 && cycle < 258) status.sprite_zero_hit = 1;
                }
                else
                {
                    if(cycle >= 1 && cycle < 258) status.sprite_zero_hit = 1;
                }
            }
        }
    }
        
    if(cycle >= 0 && cycle < 256 && scanline >= 0 && scanline < 240) screen.setPixel(cycle, scanline, getColorFromPaletteRam(palette, pixel));

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
    return palette_table[read_ppu(0x3F00 + (palette << 2) + pixel)];
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