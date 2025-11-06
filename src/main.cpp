#include "Sound.h"
#include "Object.h"
#include "Button.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){

    int screenW, screenH;
    float buttonW = 400, buttonH = 200;
    
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
            switch(e.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    snd.stop();
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(playButton.handleEvent(e)){
                        static bool playing = false;
                        if(playing){
                            snd.stop();
                        } else {
                            snd.start();
                        }
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
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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