#pragma once
#include "SDL3/SDL.h"
#include <array>
#include <glad/glad.h>

class Slider{
    public:

        /**
         * Playbutton 
         * @param shaderProgram The shader used to draw the button
         * @param x The x-coordinate of where the button is placed
         * @param y The y-coordinate of where the button is placed
         * @param h The height of the button
         * @param w The width of the button
         * @param borderR The border radius used for the slider and/or the knob to change the value
         * @param minVal The minimum allowed value
         * @param maxVal The maximum allowed value
         * @param color A four element array of Uint8 to determine the color of the button
         */
        Slider(GLuint shaderProgram, float x, float y, float w, float h, int borderR, float minVal, float maxVal, const std::array<Uint8, 4> color);
        ~Slider();

        /**
         * Handles events related to this slider
         * @param e The SDL_Event which provokes the method
         */
        bool handleEvent(const SDL_Event &e);

        /**
         * Draws the button
         * @param screenW The width of the screen
         * @param screenH The height of the screen
         */
        void draw(int screenW, int screenH);

        float getValue(){return value_;};

    private:
        GLuint shaderProgram_;
        float x_, y_, width_, height_;
        int borderRadius_;
        float minValue_, maxValue_;
        float value_= 0.0f;
        std::array<Uint8, 4> color_;
        bool dragging_ = false;
        unsigned int vbo_ = 0, vao_ = 0;

        void initBuffers();
};