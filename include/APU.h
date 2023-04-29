#ifndef APU_H
#define APU_H

#include <cstdint>
#include <functional>

struct Sequencer
{
    uint32_t sequence = 0x00000000;
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

struct OscPulse
{
    double frequency = 0.0;
    double duty = 0.0;
    double amplitude = 1.0;
    double pi = 3.14159;
    double harmonics  = 20;

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

        for(int i = 1; i <= harmonics; i++)
        {
            double c = i * pi * frequency * t;
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
    Sequencer pulse1_seq;
    OscPulse pulse1_osc;
    bool pulse1_enable = false;
    double pulse1_sample = 0.0;

private:
    uint32_t clock_counter = 0;
    uint32_t frame_clock_counter = 0;

    double globalTime = 0.0;
};

#endif