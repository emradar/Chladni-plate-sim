#pragma once
#include <vector>
#include <glad/glad.h>
#include "SDL3/SDL.h"
#include "Sound.h"
#include "Particle.h"

enum Shape{
    Square,
    Triangle,
    Circle
};

class Object {
    public:

        Object(GLuint shaderProgram, Shape s, float x, float y, float h, float w, int gridSize);
        ~Object();
        
        void update(const Sound &snd, float dt);
        void draw(int screenW, int screenH);

    private:
        GLuint shaderProgram_;
        std::vector<Particle> particles_;
        std::vector<float> particleVerts_;
        Shape shape_;
        float x_, y_, height_, width_;
        int gridSize_;
        unsigned int vao_ = 0, vbo_ = 0, particleVao_ = 0, particleVbo_ = 0;

        void initBuffers();
};