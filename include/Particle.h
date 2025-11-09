#pragma once
#include "SDL3/SDL.h"

struct Particle{

    float x, y;
    float originX, originY;
    float displacement;
    GLuint vbo, vao;
};