#include "Sound.h"
#include "Object.h"
#include "Button.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){

    int screenW, screenH;
    float buttonW = 50, buttonH = 50;
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Chladni Plate Simulator", 800, 700, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    
    SDL_GetWindowSize(window, &screenW, &screenH);
    float buttonX = screenW-buttonW, buttonY = screenH-buttonH;

    Button playButton = Button(buttonX, buttonY, buttonW, buttonH);
    Object square = Object(Shape::Square, 500.0, 500.0);
    Sound snd = Sound();
    snd.setFrequency(440);
    snd.setAmplitude(0.2);

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
                    break;
                case SDL_EVENT_KEY_DOWN:
                    auto& key = e.key.key;
                    switch(key){
                        case SDLK_ESCAPE:
                            running = false;
                            snd.stop();
                            break;
                        case SDLK_SPACE:
                            if(playButton.handleEvent(e)){
                                playing ? snd.stop() : snd.start();
                                playing = !playing;
                            }
                            break;
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        square.draw(renderer);
        playButton.draw(renderer);

        snd.tick();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}