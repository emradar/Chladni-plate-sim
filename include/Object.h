#pragma once
#include <vector>
#include "SDL3/SDL.h"

enum Shape{
    Square,
    Triangle,
    Circle
};

class Object {
    public:

        Object(Shape s, float h, float w);
        ~Object() = default;

        Shape getShape(){return shape_;}
        float getHeight(){return height_;}
        float getWidth(){return width_;}

        void drawShape(SDL_Renderer *renderer);
    private:
        Shape shape_;
        float height_;
        float width_;
};