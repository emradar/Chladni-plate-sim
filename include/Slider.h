#pragma once
#include "SDL3/SDL.h"

class Slider{
    public:

        Slider(SDL_Renderer *renderer, float x, float y, float w, float h, float borderR, float minVal, float maxVal, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
        : renderer_(renderer), x_(x), y_(y), width_(w), height_(h), borderRadius_(borderR), minValue_(minVal), maxValue_(maxVal), r_(r), g_(g), b_(b), a_(a){};

        bool handleEvent(const SDL_Event &e);
        void draw();

        float getValue(){return value_;};

    private:
        SDL_Renderer *renderer_;
        float x_, y_, width_, height_, borderRadius_;
        float minValue_, maxValue_;
        float value_= 0;
        Uint8 r_, g_, b_, a_;
        bool dragging_ = false;

        void drawRoundedRect(SDL_Renderer* renderer, float radius);
        void drawFilledCircle(SDL_Renderer* renderer, float cx, float cy, float r);
};