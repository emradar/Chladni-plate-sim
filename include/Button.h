#pragma once
#include <iostream>
#include "SDL3/SDL.h"

class Button{
    public:
        /**
         * @brief Creates a button that can be used
         * @param x = x coordinate
         * @param y = y coordinate
         * @param w = width of the button
         * @param h = height of the button
         * @param text = label of the button
         */
        Button(float x, float y, float w, float h)
        : x_(x), y_(y), width_(w), height_(h), pressed_(false) {}

        void togglePressed(){pressed_ = !pressed_;}
        bool handleEvent(const SDL_Event &e);

        void draw(SDL_Renderer *renderer);

    private:
        float x_, y_, width_, height_;
        bool pressed_;
};