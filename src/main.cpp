#include "Sound.h"
#include "Object.h"
#include "PlayButton.h"
#include "Slider.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){

    int screenW, screenH;
    float buttonW = 50, buttonH = 50;
    float sliderW = 200, sliderH = 50;

    float margin = 20.0f;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Chladni Plate Simulator", 800, 700, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    
    SDL_GetWindowSize(window, &screenW, &screenH);
    float buttonX = screenW-buttonW-margin, buttonY = screenH-buttonH-margin;
    float freqX = screenW-sliderW-margin, freqY = buttonY-sliderH-margin;
    float ampX = freqX, ampY = freqY-sliderH-margin;

    PlayButton playButton = PlayButton(renderer, buttonX, buttonY, buttonW, buttonH, 50, 50, 50, 255);
    Slider freqSlider = Slider(renderer, freqX, freqY, sliderW, sliderH, 20, 20.0f, 4000.0f, 50, 50, 50, 255);
    Slider ampSlider = Slider(renderer, ampX, ampY, sliderW, sliderH, 20, 0.0f, 1.0f, 50, 50, 50, 255);
    Object square = Object(Shape::Square, 500.0, 500.0);
    Sound snd = Sound();

    bool running = true;
    
    while (running){

        SDL_Event e;
        while(SDL_PollEvent(&e)){
            static bool playing = false;
            switch(e.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    snd.stop();
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(playButton.handleEvent(e)){
                        playing ? snd.stop() : snd.start();
                        playing = !playing;
                    }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                case SDL_EVENT_MOUSE_MOTION:
                    if(freqSlider.handleEvent(e))
                        snd.setFrequency(freqSlider.getValue());
                    
                    if(ampSlider.handleEvent(e))
                        snd.setAmplitude(ampSlider.getValue());
                        
                    break;
                case SDL_EVENT_KEY_DOWN:
                    auto& key = e.key.key;
                    switch(key){
                        case SDLK_ESCAPE:
                            running = false;
                            snd.stop();
                            break;
                        case SDLK_SPACE:
                            playing ? snd.stop() : snd.start();
                            playButton.togglePressed();
                            playing = !playing;
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        square.draw(renderer);
        playButton.draw();
        freqSlider.draw();
        ampSlider.draw();
        snd.tick();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}