#include "Object.h"

Object::Object(GLuint shaderProgram, Shape s, float x, float y, float h, float w, int gridSize)
: shaderProgram_(shaderProgram), shape_(s), x_(x), y_(y), height_(h), width_(w), gridSize_(gridSize){

    particles_.reserve(gridSize*gridSize);
    float dx = width_ / (gridSize - 1);
    float dy = height_ / (gridSize - 1);

    for(int i = 0; i < gridSize; ++i){
        for(int j = 0; j < gridSize; ++j){
            Particle p;
            p.originX = j * dx*1.5f;
            p.originY = i * dy*1.5f;
            p.displacement = 0.0f;
            particles_.push_back(p);
        }
    }

    particleVerts_.resize(particles_.size()*2);
    initBuffers();
}

Object::~Object(){
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
    if (particleVbo_) glDeleteBuffers(1, &particleVbo_);
    if (particleVao_) glDeleteVertexArrays(1, &particleVao_);
}

void Object::initBuffers(){

    // object
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // particles
    glGenVertexArrays(1, &particleVao_);
    glGenBuffers(1, &particleVbo_);
    glBindVertexArray(particleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, particleVbo_);

    glBufferData(GL_ARRAY_BUFFER,
                particleVerts_.size() * sizeof(float),
                nullptr,                 
                GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Object::update(const Sound &snd, float dt) {
    double amp = snd.getAmplitude();
    double freq = snd.getFrequency();
    static double time = 0.0;
    time += dt;

    int nx = std::clamp(int(freq / 200.0), 1, 10);
    int ny = std::clamp(int(freq / 300.0), 1, 10);

    double omega = 2.0 * SDL_PI_D * freq;
    double kx = nx * SDL_PI_D / width_;
    double ky = ny * SDL_PI_D / height_;

    float disp;
    for(size_t i = 0; i < particles_.size(); ++i){
        Particle &p = particles_[i];

        disp = static_cast<float>(
            amp * 40.0 * std::sin(omega * time) *
            std::sin(kx * p.originX) * std::sin(ky * p.originY)
        );

        p.displacement = disp;

        particleVerts_[2*i + 0] = x_ + p.originX;
        particleVerts_[2*i + 1] = y_ + p.originY + disp;

    }

    glBindVertexArray(particleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, particleVbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, particleVerts_.size() * sizeof(float), particleVerts_.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Object::draw(int screenW, int screenH){

    glUseProgram(shaderProgram_);

    GLint resLoc = glGetUniformLocation(shaderProgram_, "uResolution");
    glUniform2f(resLoc, (float)screenW, (float)screenH);

    GLint colorLoc = glGetUniformLocation(shaderProgram_, "uColor");

    // drawing the object
    glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    std::vector<float> bg;
    switch(shape_){
        case Shape::Square:{
            bg = { 
                x_, y_, 
                x_ + width_, y_, 
                x_ + width_, y_ + height_, 
                x_, y_ + height_ 
            };

            break;
        }

        case Shape::Circle:{
            float cx = x_;
            float cy = x_;
            float r = x_/2;
            int segments = 32;

            bg.push_back(cx);
            bg.push_back(cy);
            for(int i = 0; i <= segments; ++i){
                float angle = i * 2 * SDL_PI_F / segments;
                bg.push_back(cx + r * cos(angle));
                bg.push_back(cy + r * sin(angle));
            }
            break;
        }

        case Shape::Triangle:{
            bg = { x_, y_, x_, y_+height_, x_+width_, y_+height_/2.0f };
            break;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, bg.size()*sizeof(float), bg.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(bg.size() / 2));

    // drawing the particles
    glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);    
    glBindVertexArray(particleVao_);
    glBindBuffer(GL_ARRAY_BUFFER, particleVbo_);
    glDrawArrays(GL_POINTS, 0, (GLsizei)particles_.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}