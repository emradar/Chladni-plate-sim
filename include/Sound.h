#pragma once
#include "SDL3/SDL.h"
#include <atomic>
#include <functional>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <algorithm>

/**
 * Enum for the type of wave
 */
enum class WaveformType{
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise
};

class Sound{

    public:

        using WaveformFunc = std::function<float(double phase)>;

        /**
         * @param origin The origin
         * @param time The duration
         * @param amp The amplitude
         * @param freq The frequency
         * @param wave The wavelength
         * @param wform The type of wave
         */
        Sound(std::vector<int> origin, std::vector<int> time, double amp, double freq, double wave, WaveformType wform);
        
        /**
         * @brief When no arguments are given, a sine wave is produced with a = 0.01 & f = 1
         */
        Sound();
        /**
         * @brief A sound is produced with a = 0.01 & f = 1 in the given waveform
         * @param wform The type of wave
         */
        Sound(WaveformType wform);
        ~Sound();

        void start();
        void stop();
        void setFrequency(double freq);
        void setAmplitude(double amp);
        void setOrigin();

        void setWaveform(WaveformType type);

        double getFrequency() const {return frequency_;}
        double getAmplitude() const {return amplitude_;}
        std::vector<int> getOrigin() const {return origin_;}
        std::vector<int> getTime() const {return time_;}
        
        void tick();

    private:
        std::vector<int> origin_;
        std::vector<int> time_;
        std::atomic<double> amplitude_;
        std::atomic<double> frequency_;
        double wavelength_;
        double phase_{0.0};
        WaveformFunc waveform_;
        SDL_AudioStream* stream_ = nullptr;
        int samplerate_ = 44100;
        
        void initStream();
};