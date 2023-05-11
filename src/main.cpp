#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include "shader.h"
#include "camera.h"
#include "input_manager.h"
#include "chunk_renderer.h"
#include "chunk_mesh.h"
#include "resource_manager.h"
#include <array>

const std::array<GLfloat, 12> frontFace{
    0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
};

const std::array<GLfloat, 12> backFace{
    1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0,
};

const std::array<GLfloat, 12> leftFace{
    0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
};

const std::array<GLfloat, 12> rightFace{
    1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1,
};

const std::array<GLfloat, 12> topFace{
    0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0,
};

int WIDTH = 960;
int HEIGHT = 540;
unsigned TICKS_PER_SECOND = 60;
unsigned FPS = 0;
SDL_Window* window = nullptr;
SDL_GLContext mainContext;

Camera camera;

ChunkMesh mesh;

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
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    mainContext = SDL_GL_CreateContext(window);
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    //SDL_GL_LoadLibrary(NULL);

    //SDL_GL_SetSwapInterval(0);
    glEnable(GL_DEPTH_TEST);
}

void init() {
    InputManager::mapControl(InputManager::MoveForward, SDL_SCANCODE_W);
    InputManager::mapControl(InputManager::MoveLeft, SDL_SCANCODE_A);
    InputManager::mapControl(InputManager::MoveBackward, SDL_SCANCODE_S);
    InputManager::mapControl(InputManager::MoveRight, SDL_SCANCODE_D);
    InputManager::mapControl(InputManager::MoveUp, SDL_SCANCODE_SPACE);
    InputManager::mapControl(InputManager::MoveDown, SDL_SCANCODE_LSHIFT);
    
    ResourceManager::loadTexture("res/grass.png", false, "grass");

    mesh.addFace(frontFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 0, 0});
    mesh.addFace(backFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 0, 0});
    mesh.addFace(topFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 0, 0});
    mesh.addFace(leftFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 0, 0});
    mesh.addFace(rightFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 0, 0});

    mesh.addFace(frontFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 2, 0});
    mesh.addFace(backFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 2, 0});
    mesh.addFace(topFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 2, 0});
    mesh.addFace(leftFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 2, 0});
    mesh.addFace(rightFace, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 0}, {0, 2, 0});
    mesh.bufferMesh();
}

void fixedUpdate() {
}

void handleInput(float deltaTime) {
    InputManager::update();
    if (InputManager::isKeyDown(InputManager::MoveForward)) {
        camera.processKeyboard(Camera::Forward, deltaTime);
    }
    if (InputManager::isKeyDown(InputManager::MoveLeft)) {
        camera.processKeyboard(Camera::Left, deltaTime);
    }
    if (InputManager::isKeyDown(InputManager::MoveBackward)) {
        camera.processKeyboard(Camera::Backward, deltaTime);
    }
    if (InputManager::isKeyDown(InputManager::MoveRight)) {
        camera.processKeyboard(Camera::Right, deltaTime);
    }
    if (InputManager::isKeyDown(InputManager::MoveUp)) {
        camera.processKeyboard(Camera::Up, deltaTime);
    }
    if (InputManager::isKeyDown(InputManager::MoveDown)) {
        camera.processKeyboard(Camera::Down, deltaTime);
    }
}

void render(float deltaFraction, float deltaTime, ChunkRenderer& chunkRenderer) {
    handleInput(deltaTime);
    //glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.47f, 0.655f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    camera.updateProjection(WIDTH, HEIGHT);

    chunkRenderer.addChunk(mesh);
    chunkRenderer.render(camera);
    
    SDL_GL_SwapWindow(window);
}

void runGameLoop() {
    Uint64 lastTime = SDL_GetTicks64();
    double amountOfTicks = TICKS_PER_SECOND;
    double tps = 1000.0 / amountOfTicks;
    double delta = 0;
    Uint64 timer = SDL_GetTicks64();
    int frames = 0;

    SDL_SetRelativeMouseMode(SDL_TRUE); // trap the mouse to the window

    ChunkRenderer chunkRenderer;

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        double now = SDL_GetTicks64();
        delta += (now - lastTime) / tps;
        double deltaTime = (now - lastTime) / 1000.0f;
        lastTime = now;

        handleInput(deltaTime);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_MOUSEMOTION) {
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS)
                    camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    break;
                default:
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    break;
                }
            }
        }

        while (delta >= 1) {
            fixedUpdate();
            delta--;
        }
        render(static_cast<float>(delta), static_cast<float>(deltaTime), chunkRenderer);
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
    init();
    runGameLoop();
    ResourceManager::clear();
    return 0;
}