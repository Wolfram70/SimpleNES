#ifndef MAPPER_H
#define MAPPER_H

#include <cstdint>

class Mapper
{
public:
    Mapper(uint8_t n_prg_banks, uint8_t u_chr_banks): n_prg_banks(n_prg_banks), n_chr_banks(n_chr_banks) {}
    ~Mapper() {};

public:
    //virtual functions
    virtual bool read_map_cpu(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool write_map_cpu(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool read_map_ppu(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool write_map_ppu(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t n_prg_banks = 0;
    uint8_t n_chr_banks = 0;
};

class Mapper_000 : public Mapper
{
public:
    Mapper_000(uint8_t n_prg_banks, uint8_t n_chr_banks): Mapper(n_prg_banks, n_chr_banks) {}
    ~Mapper_000() {};

public:
    bool read_map_cpu(uint16_t addr, uint32_t &mapped_addr); 
    bool write_map_cpu(uint16_t addr, uint32_t &mapped_addr);
    bool read_map_ppu(uint16_t addr, uint32_t &mapped_addr);
    bool write_map_ppu(uint16_t addr, uint32_t &mapped_addr);
};

#endif