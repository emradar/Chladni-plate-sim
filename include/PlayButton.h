#pragma once
#include "SDL3/SDL.h"
#include <array>
#include <vector>
#include <glad/glad.h>

class PlayButton{
    public:

        PlayButton(GLuint shaderProgram, float x, float y, float w, float h, const std::array<Uint8,4>& color);
        ~PlayButton();

        bool handleEvent(const SDL_Event &e);
        void togglePressed(){pressed_ = !pressed_;}
        void draw(int screenW, int screenH);

    private:
        GLuint shaderProgram_;
        float x_, y_, width_, height_;
        std::array<Uint8, 4> color_;
        bool pressed_=false;
        unsigned int vao_ = 0, vbo_ = 0;

        void initBuffers();
};