#pragma once
#include "SDL3/SDL.h"

class PlayButton{
    public:

        PlayButton(SDL_Renderer *renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        : renderer_(renderer), x_(x), y_(y), width_(w), height_(h), r_(r), g_(g), b_(b), a_(a){};

        bool handleEvent(const SDL_Event &e);
        void draw();

        void togglePressed(){pressed_ = !pressed_;}
    private:
        SDL_Renderer *renderer_;
        float x_, y_, width_, height_;
        Uint8 r_, g_, b_, a_;
        bool pressed_=false;
};