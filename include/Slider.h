#pragma once
#include "SDL3/SDL.h"
#include <array>
#include <glad/glad.h>

class Slider{
    public:

        Slider(GLuint shaderProgram, float x, float y, float w, float h, int borderR, float minVal, float maxVal, const std::array<Uint8, 4> color);
        ~Slider();

        bool handleEvent(const SDL_Event &e);
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