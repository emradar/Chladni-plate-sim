#include "Slider.h"
#include "Sound.h"

bool Slider::handleEvent(const SDL_Event &e) {
    bool changed = false;

    float normalizedVal = (value_ - minValue_) / (maxValue_ - minValue_);
    float knobX = x_ + borderRadius_ + (width_ - 2 * borderRadius_) * normalizedVal;
    float knobY = y_ + height_ / 2.0f;
    float knobRadius = borderRadius_ / 2.0f;

    switch (e.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float mx = e.button.x;
            float my = e.button.y;

            float dx = mx - knobX;
            float dy = my - knobY;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance <= knobRadius) {
                dragging_ = true;
                changed = true;
            }
            break;
        }

        case SDL_EVENT_MOUSE_BUTTON_UP:
            dragging_ = false;
            break;

        case SDL_EVENT_MOUSE_MOTION:
            if (dragging_) {
                float mx = e.motion.x;
                float t = (mx - x_ - borderRadius_) / (width_ - 2 * borderRadius_);
                t = std::clamp(t, 0.0f, 1.0f);
                value_ = minValue_ + t * (maxValue_ - minValue_);
                changed = true;
            }
            break;
    }

    return changed;
}


void Slider::draw(){
    SDL_SetRenderDrawColor(renderer_, r_, g_, b_, a_);
    drawRoundedRect(renderer_, borderRadius_);
    SDL_SetRenderDrawColor(renderer_, 128, 20, 40, 255);

    float effectiveX = x_+borderRadius_;
    float effectiveY = y_+(height_-height_/5)/2;
    float normalizedVal = (value_ - minValue_) / (maxValue_ - minValue_);
    float usableWidth = (width_ - 2*borderRadius_) * normalizedVal;
    SDL_FRect bar = {effectiveX, effectiveY, usableWidth, height_/5};
    drawFilledCircle(renderer_, effectiveX + usableWidth, effectiveY + (height_/5)/2, borderRadius_/2);
    
    SDL_RenderFillRect(renderer_, &bar);
}

void Slider::drawRoundedRect(SDL_Renderer* renderer, float radius) {
    // main rectangle
    SDL_FRect center = { x_ + radius, y_, width_ - 2*radius, height_ };
    SDL_RenderFillRect(renderer, &center);

    // left and right rectangles
    SDL_FRect left = { x_, y_ + radius, radius, height_ - 2*radius };
    SDL_FRect right = { x_ + width_ - radius, y_ + radius, radius, height_ - 2*radius };
    SDL_RenderFillRect(renderer, &left);
    SDL_RenderFillRect(renderer, &right);

    // circles for each corner
    drawFilledCircle(renderer, x_ + radius,         y_ + radius,         radius);  // top-left
    drawFilledCircle(renderer, x_ + width_ - radius, y_ + radius,         radius);  // top-right
    drawFilledCircle(renderer, x_ + radius,         y_ + height_ - radius, radius); // bottom-left
    drawFilledCircle(renderer, x_ + width_ - radius, y_ + height_ - radius, radius);// bottom-right
}

void Slider::drawFilledCircle(SDL_Renderer* renderer, float cx, float cy, float r) {
    float r2 = r * r;

    for (float dy = -r; dy <= r; dy += 1.0f) {
        float dx = std::sqrt(r2 - dy*dy);  // horizontal half-width
        // draw horizontal line at this dy
        SDL_RenderLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}
