#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>
#include "game.h"

static const int START_WIDTH = 1280;//960;
static const int START_HEIGHT = 720;//540;

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
        START_WIDTH, START_HEIGHT,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);

    mainContext = SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    //SDL_GL_LoadLibrary(NULL);

    //SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
}

int main() {
    initScreen();
    Game game(window, START_WIDTH, START_HEIGHT);
    game.runGameLoop();
    return 0;
}