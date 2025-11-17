#pragma once
#include "SDL3/SDL.h"
#include <array>
#include <vector>
#include <glad/glad.h>

class PlayButton{
    public:

        /**
         * Playbutton 
         * @param shaderProgram The shader used to draw the button
         * @param x The x-coordinate of where the button is placed
         * @param y The y-coordinate of where the button is placed
         * @param h The height of the button
         * @param w The width of the button
         * @param color A four element array of Uint8 to determine the color of the button
         */
        PlayButton(GLuint shaderProgram, float x, float y, float w, float h, const std::array<Uint8,4>& color);
        ~PlayButton();

        /**
         * Handles events related to this button
         * @param e The SDL_Event which provokes the method
         */
        bool handleEvent(const SDL_Event &e);
        void togglePressed(){pressed_ = !pressed_;}
        
        /**
         * Draws the button
         * @param screenW The width of the screen
         * @param screenH The height of the screen
         */
        void draw(int screenW, int screenH);

    private:
        GLuint shaderProgram_;
        float x_, y_, width_, height_;
        std::array<Uint8, 4> color_;
        bool pressed_=false;
        unsigned int vao_ = 0, vbo_ = 0;

        void initBuffers();
};