#include "../include/APU.h"

APU::APU()
{
}

APU::~APU()
{
}

void APU::write_cpu(uint16_t addr, uint8_t data)
{
    switch(addr)
    {
        case 0x4000:
            switch((data & 0xC0) >> 6)
            {
                case 0x00:
                    pulse1_seq.sequence = 0b01000000;
                    pulse1_osc.duty = 0.125;
                    break;
                case 0x01:
                    pulse1_seq.sequence = 0b01100000;
                    pulse1_osc.duty = 0.250;
                    break;
                case 0x02:
                    pulse1_seq.sequence = 0b01111000;
                    pulse1_osc.duty = 0.500;
                    break;
                case 0x03:
                    pulse1_seq.sequence = 0b10011111;
                    pulse1_osc.duty = 0.750;
                    break;
            }
            break;
        case 0x4001:
            break;
        case 0x4002:
            pulse1_seq.reload = (pulse1_seq.reload & 0xFF00) | data;
            break;
        case 0x4003:
            pulse1_seq.reload = uint16_t)((data & 0x07) << 8) | (pulse1_seq.reload & 0x00FF);
            pulse1_seq.timer = pulse1_seq.reload;
            break;
        case 0x4004:
            break;
        case 0x4005:
            break;
        case 0x4006:    
            break;
        case 0x4007:
            break;
        case 0x4008:
            break;
        case 0x400C:
            break;
        case 0x400E:
            break;
        case 0x4015:
            pulse1_enable = (data & 0x01);
            break;
        case 0x400F:
            break;
    }
}

uint8_t APU::read_cpu(uint16_t addr)
{
    return 0;
}

void APU::clock()
{
    bool quarter_frame_clock = false;
    bool half_frame_clock = false;

    globalTime += (0.3333333333 / 1789773.0);

    if(clock_counter % 6 == 0)
    {
        frame_clock_counter++;

        if(frame_clock_counter == 3729)
        {
            quarter_frame_clock = true;
        }

        if(frame_clock_counter == 7457)
        {
            quarter_frame_clock = true;
            half_frame_clock = true;
        }

        if(frame_clock_counter == 11186)
        {
            quarter_frame_clock = true;
        }

        if(frame_clock_counter == 14916)
        {
            quarter_frame_clock = true;
            half_frame_clock = true;
            frame_clock_counter = 0;
        }

        if(quarter_frame_clock) //adjust envelope
        {
        }

        if(half_frame_clock) //adjust note length and sweep
        {
        }

        pulse1_osc.frequency = 1789773.0 / (16.0 * (double)(pulse1_seq.reload + 1));
        pulse1_sample = pulse1_osc.sample(globalTime);
    }

    clock_counter++;
}

void APU::reset()
{
}

double APU::getSample()
{
    return pulse1_sample;
}