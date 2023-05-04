#ifndef APU_H
#define APU_H

#include <cstdint>
#include <functional>

struct Sequencer
{
    uint32_t sequence = 0x00000000;
    uint32_t new_sequence = 0x00000000;
    uint16_t timer = 0x0000;
    uint16_t reload = 0x0000;
    uint8_t output = 0x00;

    uint8_t clock(bool enable, std::function<void (uint32_t &s)> doSomething)
    {
        if(enable)
        {
            timer--;
            if(timer == 0xFFFF)
            {
                timer = reload;
                doSomething(sequence);
                output = sequence & 0x00000001;
            }
        }

        return output;
    }
};

struct LengthCounter
{
    uint8_t counter = 0x00;

    uint8_t clock(bool enable, bool halt)
    {
        if(!enable)
        {
            counter = 0;
        }
        else
        {
            if(counter > 0 && !halt)
            {
                counter--;
            }
        }
        return counter;
    }
};


struct Envelope
{
    bool start = false;
    bool disable = false;
    uint16_t divider = 0x00;
    uint16_t volume = 0x00;
    uint16_t output = 0x00;
    uint16_t decay = 0x00;

    void clock(bool loop)
    {
        if(start)
        {
            decay = 15;
            start = false;
            divider = volume;
        }
        else
        {
            if(divider == 0)
            {
                divider = volume;
                if(decay == 0)
                {
                    if(loop)
                    {
                        decay = 15;
                    }
                }
                else
                {
                    decay--;
                }
            }
            else
            {
                divider--;
            }
        }

        if(disable)
        {
            output = volume;
        }
        else
        {
            output = decay;
        }
    }
};

struct FrequencySweeper
{
    bool enable = false;
    bool down = false;
    bool reload = false;
    uint8_t shift = 0x00;
    uint8_t timer = 0x00;
    uint8_t period = 0x00;
    uint16_t target_period = 0x00;
    uint16_t change = 0x00;
    bool mute = false;

    void track(uint16_t &target, bool channel)
    {
        change = target >> (shift);
        if(down)
        {
            target_period = target + (~change) + (uint16_t)channel;
        }
        else
        {
            target_period = target + change;
        }
        mute = (((target < 8) || (target_period > 0x7FF)) ? true : false);
    }

    bool clock(uint16_t &target, bool channel)
    {
        bool changed = false;
        if(timer == 0 && enable && !mute && shift != 0)
        {
            target = target_period;
            change = target >> (shift);
            if(down)
            {
                target_period = target + (~change) + (uint16_t)channel;
            }
            else
            {
                target_period = target + change;
            }
            mute = (((target < 8) || (target_period > 0x7FF)) ? true : false);
            changed = true;
        }

        if(timer == 0 || reload)
        {
            timer = period;
            reload = false;
        }
        else
        {
            timer--;
        }
        mute = (((target < 8) || (target_period > 0x7FF)) ? true : false);

        return changed;
    }
};

struct OscPulse
{
    double frequency = 0.0;
    double duty = 0.0;
    double amplitude = 1;
    double pi = 3.14159;
    double harmonics  = 10;

    double sample(double t)
    {
        double a = 0;
        double b = 0;
        double p = duty * 2 * pi;

        auto approxsin = [](float t)
        {
            float j = t * 0.15915f;
            j = j - (int)j;
            return 20.785f * j * (j - 0.5f) * (j - 1.0f);
        };

        for(int i = 1; i < harmonics; i++)
        {
            double c = i * pi * frequency * t * 2.0;
            a += - approxsin(c) / i;
            b += - approxsin(c - p * i) / i;
        }

        return (2.0 * amplitude / pi) * (a - b);
    }
};

class APU
{
public:
    APU();
    ~APU();

public:
    void write_cpu(uint16_t addr, uint8_t data);
    uint8_t read_cpu(uint16_t addr);
    void clock();
    void reset();
    double getSample();

private:
    //pulse 1
    Sequencer pulse1_seq;
    OscPulse pulse1_osc;
    Envelope pulse1_env;
    LengthCounter pulse1_len;
    FrequencySweeper pulse1_sweep;
    bool pulse1_enable = false;
    bool pulse1_halt = false;
    double pulse1_sample = 0.0;
    double pulse1_output = 0.0;

    //pulse 2
    Sequencer pulse2_seq;
    OscPulse pulse2_osc;
    Envelope pulse2_env;
    LengthCounter pulse2_len;
    FrequencySweeper pulse2_sweep;
    bool pulse2_enable = false;
    bool pulse2_halt = false;
    double pulse2_sample = 0.0;
    double pulse2_output = 0.0;

public:
    uint16_t pulse1_visual = 0;
    uint16_t pulse2_visual = 0;

private:
    uint32_t clock_counter = 0;
    uint32_t frame_clock_counter = 0;

    double globalTime = 0.0;
};

#endif