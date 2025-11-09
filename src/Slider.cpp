#include "Slider.h"
#include <glad/glad.h>
#include <GL/gl.h>
#include <algorithm>
#include <vector>
#include <cmath>

Slider::Slider(GLuint shaderProgram, float x, float y, float w, float h, int borderR, float minVal, float maxVal, const std::array<Uint8, 4> color)
: shaderProgram_(shaderProgram), x_(x), y_(y), width_(w), height_(h), borderRadius_(borderR), minValue_(minVal), maxValue_(maxVal), color_(color){
    initBuffers(); 
}

Slider::~Slider(){
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

void Slider::initBuffers(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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

void Slider::draw(int screenW, int screenH) {
    glUseProgram(shaderProgram_);
    
    GLint resLoc = glGetUniformLocation(shaderProgram_, "uResolution");
    glUniform2f(resLoc, (float)screenW, (float)screenH);
    GLint colorLoc = glGetUniformLocation(shaderProgram_, "uColor");

    // background
    std::vector<float> bg = { x_, y_, x_ + width_, y_, x_ + width_, y_ + height_, x_, y_ + height_ };

    // bar
    float normalized = (value_ - minValue_) / (maxValue_ - minValue_);
    float usable = (width_ - 2*borderRadius_) * normalized;
    float barX = x_ + borderRadius_;
    std::vector<float> fg = { barX, y_ + (height_ - height_/5.0f)/2.0f,
                              barX + usable, y_ + (height_ - height_/5.0f)/2.0f,
                              barX + usable, y_ + (height_ - height_/5.0f)/2.0f + height_/5.0f,
                              barX, y_ + (height_ - height_/5.0f)/2.0f + height_/5.0f };

    // knob
    std::vector<float> knob;
    float cx = x_+borderRadius_+usable;
    float cy = y_ + height_/2;
    float r = borderRadius_/2;
    int segments = 32;
    knob.push_back(cx);
    knob.push_back(cy);
    for(int i = 0; i <= segments; ++i){
        float angle = i * 2 * SDL_PI_F / segments;
        knob.push_back(cx + r * cos(angle));
        knob.push_back(cy + r * sin(angle));
    }

    // background
    glUniform4f(colorLoc, color_[0]/255.f, color_[1]/255.f, color_[2]/255.f, color_[3]/255.f);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, bg.size()*sizeof(float), bg.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // bar
    glUniform4f(colorLoc, 0.5f, 0.08f, 0.16f, 1.0f);
    glBufferData(GL_ARRAY_BUFFER, fg.size()*sizeof(float), fg.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // knob
    glUniform4f(colorLoc, 0.8f, 0.8f, 0.8f, 1.0f);
    glBufferData(GL_ARRAY_BUFFER, knob.size()*sizeof(float), knob.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)knob.size()/2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
