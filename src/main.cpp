#include <iostream>
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>

int WIDTH = 960;
int HEIGHT = 540;
unsigned TICKS_PER_SECOND = 60;
unsigned FPS = 0;
SDL_Window* window = nullptr;
SDL_GLContext mainContext;

void initScreen() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Video initialization failed: " << SDL_GetError();
        exit(1);
    }
    atexit(SDL_Quit);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Voxel Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    mainContext = SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    //SDL_GL_LoadLibrary(NULL);

    //SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void fixedUpdate() {

}

void render(float deltaFraction) {
    
}

void runGameLoop() {
    Uint64 lastTime = SDL_GetTicks64();
    double amountOfTicks = TICKS_PER_SECOND;
    double tps = 1000.0 / amountOfTicks;
    double delta = 0;
    Uint64 timer = SDL_GetTicks64();
    int frames = 0;

    SDL_SetRelativeMouseMode(SDL_TRUE); // trap the mouse to the window

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        double now = SDL_GetTicks64();
        delta += (now - lastTime) / tps;
        double deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                }
            }
        }

        while (delta >= 1) {
            fixedUpdate();
            delta--;
        }
        render(static_cast<float>(delta));
        ++frames;
        
        if (SDL_GetTicks64() - timer > 1000) {
            timer += 1000;
            FPS = frames;
            std::cout << "FPS: " << FPS << std::endl;
            frames = 0;
        }
    }
    SDL_DestroyWindow(window);
}

int main() {
    initScreen();
    runGameLoop();
    return 0;
}