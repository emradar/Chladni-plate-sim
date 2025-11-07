#include "PlayButton.h"
#include "Sound.h"

bool PlayButton::handleEvent(const SDL_Event &e) {

    float mx = e.button.x;
    float my = e.button.y;

    if (mx >= x_ && mx <= x_+width_ && my >= y_ && my <= y_+height_) {
        togglePressed();
        return true;
    }

    return false;
}

void PlayButton::draw(){

    SDL_SetRenderDrawColor(renderer_, r_, g_, b_, a_);
   
    if(pressed_){
        // draw a pause button (two rectangles)
        SDL_FRect rect1 = {x_, y_, width_/5, height_};   
        SDL_FRect rect2 = {x_+width_/5*3, y_, width_/5, height_};
        SDL_RenderFillRect(renderer_, &rect1);
        SDL_RenderFillRect(renderer_, &rect2);
    } else {
        // draw a play button (triangle)
        SDL_Vertex verts[3] = {
            { SDL_FPoint{ x_, y_ },           SDL_FColor{r_/255.0f, g_/255.0f, b_/255.0f, 1.0f}, SDL_FPoint{0,0} },
            { SDL_FPoint{ x_, y_ + height_ }, SDL_FColor{r_/255.0f, g_/255.0f, b_/255.0f, 1.0f}, SDL_FPoint{0,0} },
            { SDL_FPoint{ x_ + width_, y_ + height_/2 }, SDL_FColor{r_/255.0f, g_/255.0f, b_/255.0f, 1.0f}, SDL_FPoint{0,0} }
        };
        
        SDL_RenderGeometry(renderer_, nullptr, verts, 3, nullptr, 0);
    }
}
