#include "../include/Bus.h"

Bus::Bus()
{
    //clear out ram
    for(auto &i : cpu_ram)
    {
        i = 0x00;
    }

    //connect the cpu to the bus
    cpu.connectBus(this);
}

Bus::~Bus()
{
}

void Bus::write_cpu(uint16_t addr, uint8_t data)
{
    if(cartridge->write_cpu(addr, data))
    {
        //cartridge has handled the write
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF) //8KB of address range
    {
        cpu_ram[addr & 0x07FF] = data; //mirror the ram to 0x0000 - 0x07FF (2KB)
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) //8KB of address range
    {
        ppu.write_cpu(addr & 0x0007, data); //mirror the PPU registers to 0x0000 - 0x0007
    }
}

uint8_t Bus::read_cpu(uint16_t addr, bool bReadOnly)
{
    uint8_t data = 0x00;

    if(cartridge->read_cpu(addr, data))
    {
        //cartridge has handled the read
    }
    else if(addr >= 0x0000 && addr <= 0x1FFF) //8KB of address range
    {
        return cpu_ram[addr & 0x07FF]; //mirror the ram to 0x0000 - 0x07FF (2KB)
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) //8KB of address range
    {
        return ppu.read_cpu(addr & 0x0007, bReadOnly); //mirror the PPU registers to 0x0000 - 0x0007
    }

    return data;
}
void Bus::insert_cartridge(const std::shared_ptr<Cartridge> &cartridge)
{
    this->cartridge = cartridge;
    ppu.connect_cartridge(cartridge);
}

void Bus::reset()
{
    cpu.reset();
    nSystemClockCounter = 0;
}

void Bus::clock()
{
    cpu.clock();
    ppu.clock();

    nSystemClockCounter++;
}