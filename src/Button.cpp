#include "Button.h"
#include "Sound.h"

bool Button::handleEvent(const SDL_Event &e) {

    float mx = e.button.x;
    float my = e.button.y;

    if (mx >= x_ && mx <= x_+width_ && my >= y_ && my <= y_+height_) {
        pressed_ = !pressed_;
        return true;
    }

    return false;
}

void Button::draw(SDL_Renderer *renderer){

    SDL_FRect rect = {x_, y_, width_, height_};
    SDL_SetRenderDrawColor(renderer,
        pressed_ ? 0 : 50,   // R
        pressed_ ? 0 : 50,   // G
        pressed_ ? 0 : 50,   // B
        255                  // A
    );
    SDL_RenderFillRect(renderer, &rect);
    /*
    float centerX = width_/2;
    float centerY = height_/2;

    SDL_Vertex verts[3] = {
        { SDL_FPoint{ x_+width_*0.25f, y_+height_*0.25f }, SDL_FColor{255,255,255,255}, SDL_FPoint{0,0} },
        { SDL_FPoint{ x_, y_ + height_ },    SDL_FColor{255,255,255,255}, SDL_FPoint{0,0} },
        { SDL_FPoint{ x_ + width_, y_ + height_/2 }, SDL_FColor{255,255,255,255}, SDL_FPoint{0,0} }
    };

    SDL_RenderGeometry(renderer, nullptr, verts, 3, nullptr, 0);
    */
}