#include "Sound.h"
#include <random>


Sound::Sound(std::vector<int> origin, std::vector<int> time, double amp, double freq, double wave, WaveformType wform):
            origin_(origin),
            time_(time),
            wavelength_(wave)
{
    setWaveform(wform);
    amplitude_.store(amp);
    frequency_.store(freq);
    initStream(); 
}

Sound::Sound(WaveformType wform){
    setWaveform(wform);
    amplitude_.store(0.01);
    frequency_.store(1);
    initStream();  
}

Sound::Sound(){
    setWaveform(WaveformType::Sine);
    amplitude_.store(0.01);
    frequency_.store(1);
    initStream();    
}

Sound::~Sound() {
    if(stream_) SDL_DestroyAudioStream(stream_);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Sound::start() {
    if (!SDL_ResumeAudioStreamDevice(stream_))
        throw std::runtime_error(std::string("SDL_ResumeAudioStreamDevice failed: ") + SDL_GetError());
}

void Sound::stop()  { 
    if (!SDL_PauseAudioStreamDevice(stream_)) 
        throw std::runtime_error(std::string("SDL_PauseAudioStreamDevice failed: ") + SDL_GetError());
}

void Sound::setFrequency(double f) { frequency_.store(f); }
void Sound::setAmplitude(double a) { amplitude_.store(std::clamp(a, 0.0, 1.0)); }

void setOrigin();

void Sound::setWaveform(WaveformType type){
    switch (type) {
        case WaveformType::Sine:
            waveform_ = [](double phase) {
                return static_cast<float>(std::sin(phase));
            };
            break;
        case WaveformType::Square:
            waveform_ = [](double phase) {
                return static_cast<float>(
                    (std::sin(phase) >= 0.0) ? 1.0f : -1.0f
                );
            };
            break;
        case WaveformType::Triangle:
            waveform_ = [](double phase) {
                double p = phase /(2.0 * SDL_PI_D);
                return static_cast<float>(2.0 * std::abs(2.0 * (p - std::floor(p + 0.5))) - 1.0);
            };
            break;
        case WaveformType::Sawtooth:
            waveform_ = [](double phase) {
                double p = phase / (2.0 * SDL_PI_D);
                return static_cast<float>(2.0 * (p - std::floor(p + 0.5)));
            };
            break;
        case WaveformType::Noise:
            waveform_ = [](double phase) {
                static thread_local std::mt19937 rng{std::random_device{}()};
                static std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
                return dist(rng);
            };
            break;
        default:
            waveform_ = [](double phase) { 
                return static_cast<float>(std::sin(phase)); 
            };
            break;
    }
}

void Sound::tick(){
    const int frame_count = 512;
    float buffer[frame_count * 2];
    const double fs = samplerate_;
    const double amp = amplitude_.load();
    const double freq = frequency_.load();

    const int min_audio = (int)((fs * sizeof(float)) / 2);
    if (SDL_GetAudioStreamQueued(stream_) < min_audio) {
        for (int i = 0; i < frame_count; ++i) {
            double phaseInc = 2.0 * SDL_PI_D * freq / fs;
            phase_ += phaseInc;
            if (phase_ >= 2.0 * SDL_PI_D) phase_ -= 2.0 * SDL_PI_D;
        
            float sample = static_cast<float>(amp * waveform_(phase_));
            buffer[2*i + 0] = sample;
            buffer[2*i + 1] = sample;
        }
        
        SDL_PutAudioStreamData(stream_, buffer, sizeof(buffer));
    }
}

void Sound::initStream(){
    if(!SDL_InitSubSystem(SDL_INIT_AUDIO))
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

    SDL_AudioSpec want{};
    want.format = SDL_AUDIO_F32;
    want.channels = 2;
    want.freq = static_cast<int>(samplerate_);

    stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &want, nullptr, nullptr);
    if(!stream_)
        throw std::runtime_error(std::string("SDL_OpenAudioDeviceStream failed: ") + SDL_GetError());
}

