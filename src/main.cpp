#include <glad/glad.h>
#include "Sound.h"
#include "Object.h"
#include "PlayButton.h"
#include "Slider.h"
#include "UIShaders.h"
#include "Material.h"
#include <thread>
#include <chrono>
#include <iostream>

const int FRAMERATE = 60;

int main(){

    // initializing everything
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        "Chladni Plate Simulator",
        1280, 720,
        SDL_WINDOW_OPENGL
    );

    if(!window){
        std::cerr << "Failed to create window\n" << SDL_GetError()<<" \n";
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if(!glContext){
        std::cerr << "Failed to create OpenGL context\n" << SDL_GetError() << "\n";
        return -1;
    }

    if (!SDL_GL_MakeCurrent(window, glContext)) {
        std::cerr << "Failed to make context current: " << SDL_GetError() << "\n";
        return -1;
    }

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
        std::cerr << "Failed to initialize GLAD\n" << SDL_GetError() << "\n";
        return -1;
    }

    int screenW, screenH;
    SDL_GetWindowSize(window, &screenW, &screenH);
    glViewport(0, 0, screenW, screenH);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(2.0f);

    GLuint shader = createUIProgram();

    // setting element dimensions
    float objectW = 500, objectH = 500;
    float buttonW = 50, buttonH = 50;
    float sliderW = 200, sliderH = 50;
    float margin = 20.0f;  
    
    float buttonX = screenW-buttonW-margin, buttonY = screenH-buttonH-margin;
    float freqX = screenW-sliderW-margin, freqY = buttonY-sliderH-margin;
    float ampX = freqX, ampY = freqY-sliderH-margin;
    float objectX = (screenW-objectW)/2, objectY = (screenH-objectH)/2;

    const std::array<Uint8, 4> gray = {50, 50, 50, 255};

    Material::Type selected = Material::Type::Aluminium;
    Material mat(selected);

    // creating elements
    PlayButton playButton = PlayButton(shader, buttonX, buttonY, buttonW, buttonH, gray);
    Slider freqSlider = Slider(shader, freqX, freqY, sliderW, sliderH, 20, 5.0f, 5000.0f, gray);
    Slider ampSlider = Slider(shader, ampX, ampY, sliderW, sliderH, 20, 0.01f, 1.0f, gray);
    Object object = Object(shader, Shape::Square, mat, objectX, objectY, objectW, objectH, 300);
    Sound snd = Sound();

    // main loop
    bool running = true;
    static bool playing = false;
    Uint64 last = SDL_GetTicks();
    while (running){

        SDL_Event e;
        while(SDL_PollEvent(&e)){
            
            switch(e.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    snd.stop();
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if(playButton.handleEvent(e)){
                        playing ? snd.stop() : snd.start();
                        playing = !playing;
                    }

                case SDL_EVENT_MOUSE_BUTTON_UP:
                case SDL_EVENT_MOUSE_MOTION:
                    if(freqSlider.handleEvent(e))
                        snd.setFrequency(freqSlider.getValue());
                    
                    if(ampSlider.handleEvent(e))
                        snd.setAmplitude(ampSlider.getValue());
                        
                    break;

                case SDL_EVENT_KEY_DOWN:
                    auto& key = e.key.key;
                    switch(key){
                        case SDLK_ESCAPE:
                            running = false;
                            snd.stop();
                            break;

                        case SDLK_SPACE:
                            playing ? snd.stop() : snd.start();
                            playButton.togglePressed();
                            playing = !playing;
                            break;
                    }
            }
        }

        Uint64 now = SDL_GetTicks();
        float dt = (now-last) / 1000.0f;
        last = now;
        double t = now /1000.0;

        // the particles should only get affected if there is a sound
        if(playing) object.update(snd, dt);

        snd.tick();

        // resetting the screen
        SDL_GetWindowSize(window, &screenW, &screenH);
        glViewport(0, 0, screenW, screenH);
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        // drawing the objects
        object.draw(screenW, screenH);
        playButton.draw(screenW, screenH);
        freqSlider.draw(screenW, screenH);
        ampSlider.draw(screenW, screenH);

        SDL_GL_SwapWindow(window);        
    }

    // destroying everything
    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}