#include "Object.h"

Object::Object(Shape s, float h, float w):shape_(s), height_(h), width_(w){
}

void Object::draw(SDL_Renderer *renderer){
    SDL_Window *window = SDL_GetRenderWindow(renderer);

    int screenW, screenH;
    SDL_GetWindowSize(window, &screenW, &screenH);

    float x = (screenW - width_) / 2.0f;
    float y = (screenH - height_) / 2.0f;

    SDL_FRect rect = {x, y, width_, height_};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}