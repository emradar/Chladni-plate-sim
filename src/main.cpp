#include "Sound.h"
int main(){
    
    static SDL_Window *window = NULL;
    static SDL_Renderer *renderer = NULL;

    SDL_CreateWindowAndRenderer("Chladni Plate Simulator", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    

}