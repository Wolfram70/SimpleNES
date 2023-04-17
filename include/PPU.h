#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>

class Cartridge;

union Status_
{
    struct 
    {
        uint8_t unused : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zero_hit : 1;
        uint8_t vertical_blank : 1;
    };
    uint8_t reg = 0x0000;
};

union Mask
{
    struct
    {
        uint8_t grayscale : 1;
        uint8_t render_background_left : 1;
        uint8_t render_sprites_left : 1;
        uint8_t render_background : 1;
        uint8_t render_sprites : 1;
        uint8_t enhance_red : 1;
        uint8_t enhance_green : 1;
        uint8_t enhance_blue : 1;
    };
    uint8_t reg = 0x0000;
};

union Control
{
    struct
    {
        uint8_t nametable_x : 1;
        uint8_t nametable_y : 1;
        uint8_t increment_mode : 1;
        uint8_t pattern_sprite : 1;
        uint8_t pattern_background : 1;
        uint8_t sprite_size : 1;
        uint8_t slave_mode : 1;
        uint8_t enable_nmi : 1;
    };
    uint8_t reg = 0x0000;
};

union Loopy_Register
{
    struct
    {
        uint16_t coarse_x : 5;
        uint16_t coarse_y : 5;
        uint16_t nametable_x : 1;
        uint16_t nametable_y : 1;
        uint16_t fine_y : 3;
        uint16_t unused : 1;
    };
    uint16_t reg = 0x0000;
};

struct Sprite
{
    uint8_t y;
    uint8_t id;
    uint8_t attributes;
    uint8_t x;
};


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
    uint8_t pattern[2][4096]; //8KB of pattern memory

private:
    sf::Color palette_table[0x40]; //64 colors in the palette table
    sf::Image screen; //the screen texture
    sf::Image name_table[2]; //the name table textures
    sf::Image pattern_table[2]; //the pattern table textures

private:
    //registers
    Status_ status;
    Mask mask;
    Control control;
    Loopy_Register vram_addr;
    Loopy_Register tram_addr;

    uint8_t address_latch = 0x00;
    uint8_t ppu_data_buffer = 0x00;
    uint8_t fine_x = 0x00;
    uint16_t ppu_address = 0x0000;
    uint8_t oam_address = 0x00;

private:
    //background rendering
    uint8_t bg_next_tile_id = 0x00;
    uint8_t bg_next_tile_attrib = 0x00;
    uint8_t bg_next_tile_lsb = 0x00;
    uint8_t bg_next_tile_msb = 0x00;

    //shifting registers
    uint16_t bg_shifter_pattern_lo = 0x0000;
    uint16_t bg_shifter_pattern_hi = 0x0000;
    uint16_t bg_shifter_attrib_lo = 0x0000;
    uint16_t bg_shifter_attrib_hi = 0x0000;

    //sprite rendering

    Sprite OAM[64]; //the sprite memory
    Sprite sprite_scanline[8]; //the sprites on the current scanline
    uint8_t sprite_count = 0x00;
    uint8_t sprite_shifter_pattern_lo[8];
    uint8_t sprite_shifter_pattern_hi[8];

    bool sprite_zero_hit_possible = false;
    bool sprite_zero_being_rendered = false;

public:
    //exposing base pointer of OAM
    uint8_t* pOAM = (uint8_t*)OAM;

public:
    int16_t scanline = 0; //the current scanline
    int16_t cycle = 0; //the current cycle

    bool nmi = false;

public:
    bool frame_complete = false; //indicates whether the current frame is complete

public:
    //helper funcs
    sf::Image& getScreen() { return screen; }
    sf::Image& getNameTable(uint8_t i) { return name_table[i]; }
    sf::Image& getPatternTable(uint8_t i, uint8_t palette);

    sf::Color& getColorFromPaletteRam(uint8_t palette, uint8_t pixel);

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
    void reset();
};

#endif