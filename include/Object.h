#pragma once
#include <vector>
#include <glad/glad.h>
#include "SDL3/SDL.h"
#include "Sound.h"
#include "Particle.h"
#include "Material.h"

/**
 * An enum for available shapes
 */
enum Shape{
    Square,
    Triangle,
    Circle
};

class Object {
    public:
        /**
         * The object on which particles are drawn. I.e. the actual Chladni Plate.
         * @param shaderProgram The shader used to draw the object
         * @param s The shape of the object
         * @param x The x-coordinate of where the object is placed
         * @param y The y-coordinate of where the object is placed
         * @param h The height of the object
         * @param w The width of the object
         * @param gridSize The size of the grid where particles are placed
         */
        Object(GLuint shaderProgram, Shape s, const Material &mat, float x, float y, float h, float w, int gridSize);
        ~Object();
        
        /**
         * Updates the particles' placements
         * @param snd The sound source
         * @param dt The difference in time from start to now
         */
        void update(const Sound &snd, float dt);

        /**
         * Draws the object and its particles
         * @param screenW The width of the screen
         * @param screenH The height of the screen
         */
        void draw(int screenW, int screenH);

        void setMaterial(const Material &mat){material_ = mat;};

        bool handleEvent(const SDL_Event &e);

        bool toggleClampedEdges(bool clamp){fullyClampedEdges_ = !fullyClampedEdges_;};

    private:
        GLuint shaderProgram_;
        Material material_;
        std::vector<Particle> particles_;
        std::vector<float> particleVerts_;
        Shape shape_;
        float x_, y_, height_, width_;
        int gridSize_;
        unsigned int vao_ = 0, vbo_ = 0, particleVao_ = 0, particleVbo_ = 0;
        std::vector<std::pair<float, float>> clampedPoints_;
        bool fullyClampedEdges_ = false;

        void initBuffers();
};