#include "Sound.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){
    Sound snd = Sound();
    snd.setWaveform(WaveformType::Sawtooth);
    snd.setFrequency(440);
    snd.start();

    bool running = true;
    SDL_Event event;
    while (running){

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT) running = false;
        }

        snd.tick();
    }
}