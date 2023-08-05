#pragma once
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
#include "chunk_map.h"
#include "world_mesh_builder.h"
#include "perlin_generator.h"
#include "block_database.h"

class Game {
public:
    Game(SDL_Window* window, int WIDTH, int HEIGHT);

    void runGameLoop();

    static const unsigned TICKS_PER_SECOND = 60;

private:
    void fixedUpdate();
    void render(float deltaFraction, float deltaTime);
    void handleInput(float deltaTime);
    unsigned FPS = 0;

    int WIDTH, HEIGHT;

    int timer = 0;
    int counter = 0;

    Camera camera;
    ChunkRenderer chunkRenderer;
    ChunkMap chunkMap;
    WorldMeshBuilder worldMesh{&chunkMap, &chunkRenderer};
    SDL_Window* window;
};