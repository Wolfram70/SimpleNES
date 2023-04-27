#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>
#include <vector>

class Mapper
{
public:
    Mapper(uint16_t n_prg_banks, uint16_t u_chr_banks): n_prg_banks(n_prg_banks), n_chr_banks(n_chr_banks) {}
    ~Mapper() {};

public:
    enum MIRROR
    {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI
    } mirror = HORIZONTAL;

public:
    //virtual functions
    virtual bool read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data) = 0;
    virtual bool write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) = 0;
    virtual bool read_map_ppu(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool write_map_ppu(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual void reset() {};
    void set_mirror(MIRROR mirror) { this->mirror = mirror; }
    MIRROR get_mirror() { return mirror; }

protected:
    uint16_t n_prg_banks = 0;
    uint16_t n_chr_banks = 0;
};

class Mapper_000 : public Mapper
{
public:
    Mapper_000(uint16_t n_prg_banks, uint16_t n_chr_banks): Mapper(n_prg_banks, n_chr_banks) {
        PRG_RAM.resize(32 * 1024);
    }
    ~Mapper_000() {};

private:
    std::vector<uint8_t> PRG_RAM;

public:
    bool read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data) override; 
    bool write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
    bool read_map_ppu(uint16_t addr, uint32_t &mapped_addr) override;
    bool write_map_ppu(uint16_t addr, uint32_t &mapped_addr) override;

    void reset() override {}
};

class Mapper_001 : public Mapper
{
    //the chr_ram is present on the cartridge as 8KB chr rom itself (since thats the max, if any, for mapper 001 also iNES header)
public:
    Mapper_001(uint16_t n_prg_banks, uint16_t n_chr_banks, uint32_t prg_ram_size): Mapper(n_prg_banks, n_chr_banks) {
        PRG_RAM.resize(prg_ram_size);
    }
    ~Mapper_001() {};

private:
    //registers
    int8_t SR = 0x10;
    uint8_t CR = 0x0C;
    uint8_t CHR0 = 0x00;
    uint8_t CHR1 = 0x00;
    uint8_t PRG = 0x00;

    std::vector<uint8_t> PRG_RAM;

public:
    bool read_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t &data) override; 
    bool write_map_cpu(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
    bool read_map_ppu(uint16_t addr, uint32_t &mapped_addr) override;
    bool write_map_ppu(uint16_t addr, uint32_t &mapped_addr) override;
    
    void reset() override;
};

#endif