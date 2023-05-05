#include "../include/APU.h"

uint8_t length_table[] = {10, 254, 20,  2, 40,  4, 80,  6, 160, 8, 60, 10, 14, 12, 26, 14, 12,  16, 24, 18, 48, 20, 96, 22, 192,  24, 72, 26, 16, 28, 32, 30 };

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
                    pulse1_seq.new_sequence = 0b01000000;
                    pulse1_osc.duty = 0.125;
                    break;
                case 0x01:
                    pulse1_seq.new_sequence = 0b01100000;
                    pulse1_osc.duty = 0.250;
                    break;
                case 0x02:
                    pulse1_seq.new_sequence = 0b01111000;
                    pulse1_osc.duty = 0.500;
                    break;
                case 0x03:
                    pulse1_seq.new_sequence = 0b10011111;
                    pulse1_osc.duty = 0.750;
                    break;
            }
            pulse1_seq.sequence = pulse1_seq.new_sequence;
            pulse1_halt = (data & 0x20);
            pulse1_env.volume = (data & 0x0F);
            pulse1_env.disable = (data & 0x10);
            break;
        case 0x4001:
            pulse1_sweep.enable = (data & 0x80);
            pulse1_sweep.period = (data & 0x70) >> 4;
            pulse1_sweep.down = (data & 0x08);
            pulse1_sweep.shift = (data & 0x07);
            pulse1_sweep.reload = true;
            break;
        case 0x4002:
            pulse1_seq.reload = (pulse1_seq.reload & 0xFF00) | data;
            pulse1_sweep.track(pulse1_seq.reload, 0);
            break;
        case 0x4003:
            pulse1_seq.reload = uint16_t((data & 0x07) << 8) | (pulse1_seq.reload & 0x00FF);
            pulse1_sweep.track(pulse1_seq.reload, 0);
            pulse1_seq.timer = pulse1_seq.reload;
            pulse1_seq.sequence = pulse1_seq.new_sequence;
            pulse1_len.counter = length_table[(data & 0xF8) >> 3];
            pulse1_env.start = true;
            break;
        case 0x4004:
            switch((data & 0xC0) >> 6)
            {
                case 0x00:
                    pulse2_seq.new_sequence = 0b01000000;
                    pulse2_osc.duty = 0.125;
                    break;
                case 0x01:
                    pulse2_seq.new_sequence = 0b01100000;
                    pulse2_osc.duty = 0.250;
                    break;
                case 0x02:
                    pulse2_seq.new_sequence = 0b01111000;
                    pulse2_osc.duty = 0.500;
                    break;
                case 0x03:
                    pulse2_seq.new_sequence = 0b10011111;
                    pulse2_osc.duty = 0.750;
                    break;
            }
            pulse2_seq.sequence = pulse2_seq.new_sequence;
            pulse2_halt = (data & 0x20);
            pulse2_env.volume = (data & 0x0F);
            pulse2_env.disable = (data & 0x10);
            break;
        case 0x4005:
            pulse2_sweep.enable = (data & 0x80);
            pulse2_sweep.period = (data & 0x70) >> 4;
            pulse2_sweep.down = (data & 0x08);
            pulse2_sweep.shift = (data & 0x07);
            pulse2_sweep.reload = true;
            break;
        case 0x4006:
            pulse2_seq.reload = (pulse2_seq.reload & 0xFF00) | data;
            pulse2_sweep.track(pulse2_seq.reload, 1);
            break;
        case 0x4007:
            pulse2_seq.reload = uint16_t((data & 0x07) << 8) | (pulse2_seq.reload & 0x00FF);
            pulse2_sweep.track(pulse2_seq.reload, 1);
            pulse2_seq.timer = pulse2_seq.reload;
            pulse2_seq.sequence = pulse2_seq.new_sequence;
            pulse2_len.counter = length_table[(data & 0xF8) >> 3];
            pulse2_env.start = true;
            break;
        case 0x4008:
            triangle_seq.halt = (data & 0x80);
            triangle_seq.reload = (data & 0x7F);
            break;
        case 0x400A:
            triangle_seq.timer_reload = (triangle_seq.timer_reload & 0xFF00) | data;
            break;
        case 0x400B:
            triangle_seq.timer_reload = (triangle_seq.timer_reload & 0x00FF) | (uint16_t)((data & 0x07) << 8);
            triangle_seq.length_counter = length_table[(data & 0xF8) >> 3];
            triangle_seq.reload_flag = true;
            break;
        case 0x400C:
            break;
        case 0x400E:
            break;
        case 0x4015:
            pulse1_enable = (data & 0x01);
            pulse2_enable = (data & 0x02);
            triangle_enable = (data & 0x04);
            if(!triangle_enable)
                triangle_seq.length_counter = 0;
            break;
        case 0x400F:
            pulse1_env.start = true;
            pulse2_env.start = true;
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

    if(clock_counter % 3 == 0)
    {
        triangle_seq.clock();
    }

    if(clock_counter % 6 == 0)
    {

        pulse1_sweep.track(pulse1_seq.reload, 0);
        pulse2_sweep.track(pulse2_seq.reload, 1);

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
            pulse1_env.clock(pulse1_halt);
            pulse2_env.clock(pulse2_halt);

            triangle_seq.linear_clock();
        }

        if(half_frame_clock) //adjust note length and sweep
        {
            pulse1_len.clock(pulse1_enable, pulse1_halt);
            pulse1_sweep.clock(pulse1_seq.reload, 0);

            pulse2_len.clock(pulse2_enable, pulse2_halt);
            pulse2_sweep.clock(pulse2_seq.reload, 1);

            triangle_seq.length_clock();
        }

        //pulse 1

        pulse1_seq.clock(pulse1_enable, [](uint32_t &s)
        {
            s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
        });

        pulse1_osc.frequency = 1789773.0 / (16.0 * (double)(pulse1_seq.reload + 1));
        pulse1_osc.amplitude = (double)(pulse1_env.output - 1) / 16.0;
        pulse1_sample = pulse1_osc.sample(globalTime);

        if(pulse1_len.counter > 0 && pulse1_seq.timer >= 8 && !pulse1_sweep.mute && pulse1_env.output > 2)
        {
            pulse1_output += (pulse1_sample - pulse1_output) * 0.5;
        }
        else
        {
            pulse1_output = 0;
            pulse1_sample = 0;
        }

        if(!pulse1_enable)
        {
            pulse1_output = 0;
            pulse1_sample = 0;
        }

        //pulse 2

        pulse2_seq.clock(pulse2_enable, [](uint32_t &s)
        {
            s = ((s & 0x0001) << 7) | ((s & 0x00FE) >> 1);
        });

        pulse2_osc.frequency = 1789773.0 / (16.0 * (double)(pulse2_seq.reload + 1));
        pulse2_osc.amplitude = (double)(pulse2_env.output - 1) / 16.0;
        pulse2_sample = pulse2_osc.sample(globalTime);

        if(pulse2_len.counter > 0 && pulse2_seq.timer >= 8 && !pulse2_sweep.mute && pulse2_env.output > 2)
        {
            pulse2_output += (pulse2_sample - pulse2_output) * 0.5;
        }
        else
        {
            pulse2_output = 0;
            pulse2_sample = 0;
        }

        if(!pulse2_enable)
        {
            pulse2_output = 0;
            pulse2_sample = 0;
        }

        //triangle

        triangle_osc.frequency = 1789773.0 / (32.0 * (double)(triangle_seq.timer_reload + 1));
        triangle_osc.amplitude = (double)(15 - 1) / 16.0;
        triangle_sample = triangle_osc.sample(globalTime);

        if(triangle_enable && triangle_seq.timer_reload > 2 && triangle_seq.length_counter > 0)
        {
            triangle_output += (triangle_sample - triangle_output) * 0.5;
        }
        else
        {
            triangle_output = 0;
            triangle_sample = 0;
        }
    }

    pulse1_visual = (pulse1_enable && pulse1_env.output > 1 && !pulse1_sweep.mute ? pulse1_seq.reload : 2047);

    pulse2_visual = (pulse2_enable && pulse2_env.output > 1 && !pulse2_sweep.mute ? pulse2_seq.reload : 2047);

    clock_counter++;
}

void APU::reset()
{
}

double APU::getSample()
{
    // return ((1.0 * pulse1_output) - 0.5) * 0.1 +
    //         ((1.0 * pulse2_output) - 0.5) * 0.1 +
    //         ((1.0 * triangle_output) - 0.5) * 0.1;
    return (pulse1_sample + pulse2_sample + triangle_sample - 1.5) * 0.1;
}