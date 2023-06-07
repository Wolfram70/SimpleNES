#include "../include/Bus.h"
#include <iostream>

#define PPU_CLOCK_FREQ 5369318.0

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
    else if(addr >= 0x2000 && addr <= 0x3FFF) //8KB of address range
    {
        ppu.write_cpu(addr & 0x0007, data); //mirror the PPU registers to 0x0000 - 0x0007
    }
    else if((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015) //REMINDER: what about controller 2?
    {
        apu.write_cpu(addr, data);
    }
    else if(addr == 0x4014) //DMA
    {
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if(addr >= 0x4016 && addr <= 0x4017)
    {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
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
    else if(addr == 0x4015)
    {
        return apu.read_cpu(addr);
    }
    else if(addr >= 0x4016 && addr <= 0x4017)
    {
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
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
    ppu.reset();
    cartridge->reset();
    nSystemClockCounter = 0;
}

bool Bus::clock()
{
    ppu.clock();

    apu.clock();

    if(nSystemClockCounter % 3 == 0)
    {
        if(dma_transfer)
        {
            if(dma_dummy)
            {
                if(nSystemClockCounter % 2 == 1) dma_dummy = false;
            }
            else
            {
                if(nSystemClockCounter % 2 == 0)
                {
                    dma_data = read_cpu((uint16_t(dma_page) << 8) | uint16_t(dma_addr));

                }
                else
                {
                    ppu.pOAM[dma_addr] = dma_data;
                    dma_addr++;

                    if(dma_addr == 0x00)
                    {
                        dma_transfer = false;
                        dma_dummy = true;
                    }
                }
            }
        }
        else
        {
            cpu.clock();
        }
    }

    bool sampleReady = false;
    timeSinceLastSample += timePerClock;

    if(timeSinceLastSample >= timePerSample)
    {
        timeSinceLastSample -= timePerSample;
        audio_sample = apu.getSample();
        sampleReady = true;
    }

    if(ppu.nmi)
    {
        ppu.nmi = false;
        cpu.nmi();
    }

    nSystemClockCounter++;

    return sampleReady;
}

void Bus::setSampleFrequency(uint32_t sample_rate)
{
    timePerSample = 1.0 / double(sample_rate);
    timePerClock = 1.0 / double(PPU_CLOCK_FREQ);
}