#ifndef NES_SOUND_H
#define NES_SOUND_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>

#include <alsa/asoundlib.h>

namespace NES
{
    class Sound
    {
    public:
        static bool create(unsigned int sampleRate, unsigned int channels, unsigned int blocks, unsigned int blockSamples);
        static bool destroy();

    public:
        static void audio();
        static float getMixOut(int nChannel, float fGlobalTime, float fTimeStep);
        static void setUserFunc(float(*func)(int, float, float));

    private:
        static float(*m_userFunction)(int, float, float);

        static unsigned int m_nSampleRate;
        static unsigned int m_nChannels;
        static unsigned int m_nBlockSamples;

        static short* m_pBlockMemory;
        static snd_pcm_t *m_pcmHandle;

        static std::thread m_thread;
        static std::atomic<bool> m_bReady;

        static std::atomic<float> m_fGlobalTime;

    };
}

namespace NES
{
    std::thread Sound::m_thread;
    std::atomic<bool> Sound::m_bReady(false);
    std::atomic<float> Sound::m_fGlobalTime(0.0f);
    float(*Sound::m_userFunction)(int, float, float) = nullptr;

    snd_pcm_t *Sound::m_pcmHandle = nullptr;
    unsigned int Sound::m_nSampleRate = 0;
    unsigned int Sound::m_nChannels = 0;
    unsigned int Sound::m_nBlockSamples = 0;
    short* Sound::m_pBlockMemory = nullptr;

    float Sound::getMixOut(int nChannel, float fGlobalTime, float fTimeStep)
    {
        float mixerSample = 0.0f;

        if(m_bReady == false)
            return mixerSample;

        if(m_userFunction != nullptr)
        {
            mixerSample += m_userFunction(nChannel, fGlobalTime, fTimeStep);
        }

        return mixerSample;
    }

    void Sound::setUserFunc(float(*func)(int, float, float))
    {
        m_userFunction = func;
    }

    void Sound::audio()
    {
        m_fGlobalTime = 0.0f;
        static float fTimeStep = 1.0f / (float)m_nSampleRate;

        short maxSample = (short) pow(2, sizeof(short) * 8 - 1) - 1;
        float fMaxSample = (float) maxSample;
        short previousSample = 0;

        while(m_bReady)
        {
            short newSample = 0;

            auto clip = [](float fSample, float fMax)
            {
                if(fSample >= 0.0)
                    return fmin(fSample, fMax);
                else
                    return fmax(fSample, -fMax);
            };

            for(unsigned int n = 0; n < m_nBlockSamples; n += m_nChannels)
            {
                for(unsigned int c = 0; c < m_nChannels; c++)
                {
                    newSample = (short)(getMixOut(c, m_fGlobalTime + fTimeStep * (float)n, fTimeStep) * fMaxSample);
                    m_pBlockMemory[n + c] = newSample;
                    previousSample = newSample;
                }
            }
            
            m_fGlobalTime = m_fGlobalTime + fTimeStep * (float) m_nBlockSamples;

            snd_pcm_uframes_t nLeft = m_nBlockSamples;
            short *pBlockPos = m_pBlockMemory;
            while(nLeft > 0)
            {
                int rc = snd_pcm_writei(m_pcmHandle, pBlockPos, nLeft);
                if(rc > 0)
                {
                    pBlockPos += rc * m_nChannels;
                    nLeft -= rc;
                }
                if(rc == -EAGAIN) continue;
                if(rc == -EPIPE)
                {
                    snd_pcm_prepare(m_pcmHandle);
                }
            }
        }
    }

    bool Sound::create(unsigned int sampleRate, unsigned int channels, unsigned int blocks, unsigned int blockSamples)
    {
        m_bReady = false;
        m_nSampleRate = sampleRate;
        m_nChannels = channels;
        m_nBlockSamples = blockSamples;
        m_pBlockMemory = nullptr;

        m_userFunction = nullptr;

        int rc = snd_pcm_open(&m_pcmHandle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if(rc < 0)
        {
            std::cout << "ERROR: Can't open default PCM device: " << snd_strerror(rc) << std::endl;
            return destroy();
        }
        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(m_pcmHandle, params);

        snd_pcm_hw_params_set_access(m_pcmHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(m_pcmHandle, params, SND_PCM_FORMAT_S16_LE);
        snd_pcm_hw_params_set_rate(m_pcmHandle, params, m_nSampleRate, 0);
        snd_pcm_hw_params_set_channels(m_pcmHandle, params, m_nChannels);
        snd_pcm_hw_params_set_period_size(m_pcmHandle, params, m_nBlockSamples, 0);
        snd_pcm_hw_params_set_periods(m_pcmHandle, params, blocks, 0);

        rc = snd_pcm_hw_params(m_pcmHandle, params);
        if(rc < 0) 
            return destroy();

        m_pBlockMemory = new short[m_nBlockSamples];

        if (m_pBlockMemory == nullptr)
            return destroy();
        std::fill(m_pBlockMemory, m_pBlockMemory + m_nBlockSamples, 0);

        snd_pcm_start(m_pcmHandle);
        for(unsigned int i = 0; i < blocks; i++)
            rc = snd_pcm_writei(m_pcmHandle, m_pBlockMemory, 512);

        snd_pcm_start(m_pcmHandle);

        m_bReady = true;
        m_thread = std::thread(&Sound::audio);

        return true;
    }

    bool Sound::destroy()
    {
        m_bReady = false;
        if(m_thread.joinable())
            m_thread.join();
        snd_pcm_drain(m_pcmHandle);
        snd_pcm_close(m_pcmHandle);
        return true;
    }
}

#endif