#include "PlayButton.h"

PlayButton::PlayButton(GLuint shaderProgram, float x, float y, float w, float h, const std::array<Uint8,4>& color)
: shaderProgram_(shaderProgram), x_(x), y_(y), width_(w), height_(h), color_(color){

    initBuffers();
}

PlayButton::~PlayButton(){
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}

void PlayButton::initBuffers() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool PlayButton::handleEvent(const SDL_Event &e) {

    float mx = e.button.x;
    float my = e.button.y;

    if (mx >= x_ && mx <= x_+width_ && my >= y_ && my <= y_+height_) {
        togglePressed();
        return true;
    }

    return false;
}

void PlayButton::draw(int screenW, int screenH) {

    glUseProgram(shaderProgram_);

    GLint resLoc = glGetUniformLocation(shaderProgram_, "uResolution");
    glUniform2f(resLoc, (float)screenW, (float)screenH);
    GLint colorLoc = glGetUniformLocation(shaderProgram_, "uColor");
    glUniform4f(colorLoc,
                color_[0]/255.0f,
                color_[1]/255.0f,
                color_[2]/255.0f,
                color_[3]/255.0f);

    std::vector<float> verts;
    if (pressed_) {
        float barW = width_/5.0f;
        verts = {
            x_, y_, x_+barW, y_, x_+barW, y_+height_,
            x_, y_, x_+barW, y_+height_, x_, y_+height_,

            x_+3*barW, y_, x_+4*barW, y_, x_+4*barW, y_+height_,
            x_+3*barW, y_, x_+4*barW, y_+height_, x_+3*barW, y_+height_
        };
    } else {
        verts = { x_, y_, x_, y_+height_, x_+width_, y_+height_/2.0f };
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(float), verts.data(), GL_DYNAMIC_DRAW);

    if (pressed_)
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)verts.size()/2);
    else
        glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


