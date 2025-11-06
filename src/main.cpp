#include "Sound.h"
#include "Object.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){

    SDL_Window *window = SDL_CreateWindow("Chladni Plate Simulator", 800, 700, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    SDL_Init(SDL_INIT_VIDEO);

    Object square = Object(Shape::Square, 500.0, 500.0);
    Sound snd = Sound();
    snd.setWaveform(WaveformType::Sawtooth);
    snd.setFrequency(440);
    snd.setAmplitude(0.2);
    snd.start();

    bool running = true;
    
    while (running){

        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    snd.stop();
                    break;
                case SDL_EVENT_KEY_DOWN:
                    auto& key = e.key.key;
                    switch(key){
                        case SDLK_ESCAPE:
                            running = false;
                            snd.stop();
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        snd.tick();
        square.drawShape(renderer);

        SDL_UpdateWindowSurface(window);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}