#include "game.h"

Game::Game(SDL_Window* window, int WIDTH, int HEIGHT)
     : window{window}, WIDTH{WIDTH}, HEIGHT{HEIGHT} {
    InputManager::mapControl(InputManager::MoveForward, SDL_SCANCODE_W);
    InputManager::mapControl(InputManager::MoveLeft, SDL_SCANCODE_A);
    InputManager::mapControl(InputManager::MoveBackward, SDL_SCANCODE_S);
    InputManager::mapControl(InputManager::MoveRight, SDL_SCANCODE_D);
    InputManager::mapControl(InputManager::MoveUp, SDL_SCANCODE_SPACE);
    InputManager::mapControl(InputManager::MoveDown, SDL_SCANCODE_LSHIFT);

    PerlinGenerator::initialize(123);
    
    // ResourceManager::loadTexture("res/grass.png", false, "grass");
    // ResourceManager::loadTexture("res/atlas.png", false, "atlas");

    ResourceManager::loadTextureArray({"res/grass.png", "res/stone.png", "res/dirt.png", "res/bedrock.png"}, false, "atlas");

    BlockDatabase::addBlockData(Grass, {0, 0});
    BlockDatabase::addBlockData(Stone, {1, 0});

    //chunkMap.addChunk(new Chunk(0, 0));
    int mapsize = 10;
    for (int i = -mapsize / 2; i <= mapsize/2; ++i) {
        for (int j = -mapsize / 2; j <= mapsize/2; ++j) {
            Chunk* chunk = new Chunk(i, j);
            chunk->initialize();
            chunkMap.addChunk(chunk);
            // if (i != 0 && i != mapsize-1 && j != 0 && j != mapsize-1)
            //     worldMesh.buildMesh({i, j});
        }
    }
    // worldMesh.buildMeshes();

    camera.setPosition({0, 64, 0});

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void Game::handleInput(float deltaTime) {
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

void Game::runGameLoop() {
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
            } else if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    WIDTH = event.window.data1;
                    HEIGHT = event.window.data2;
                }
            }
        }

        while (delta >= 1) {
            fixedUpdate();
            delta--;
        }
        render(static_cast<float>(delta), static_cast<float>(deltaTime));
        ++frames;
        
        if (SDL_GetTicks64() - timer > 1000) {
            timer += 1000;
            FPS = frames;
            std::cout << "FPS: " << FPS << std::endl;
            frames = 0;
        }
    }
    SDL_DestroyWindow(window);
    ResourceManager::clear();
}

void Game::render(float deltaFraction, float deltaTime) {
    handleInput(deltaTime);
    //glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.47f, 0.655f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    camera.updateProjection(WIDTH, HEIGHT);

    worldMesh.render(camera);
    
    SDL_GL_SwapWindow(window);
}

void Game::fixedUpdate() {
    // if (timer > 20) {
    // chunkMap.addChunk(new Chunk(-counter, 0));
    // worldMesh.buildMesh({-counter+1, 0});
    // worldMesh.buildMesh({-counter, 0});
    // counter++;
    // timer = 0;
    // }
    // timer++;

    glm::vec3 pos = camera.getPosition();
    int x = pos.x;
    int z = pos.z;
    int chunkX = x >= 0 ? x / CHUNK_SIZE : (x - CHUNK_SIZE + 1) / CHUNK_SIZE;
	int chunkZ = z >= 0 ? z / CHUNK_SIZE : (z - CHUNK_SIZE + 1) / CHUNK_SIZE;
    int radius = 8;
    chunkMap.addChunkRadius({chunkX, chunkZ}, radius+1);
    for (int i = chunkX - radius; i <= chunkX + radius; ++i) {
        for (int j = chunkZ - radius; j <= chunkZ + radius; ++j) {
            // if (chunkMap.getChunk(i, j) == nullptr) {
			// 	chunkMap.addChunk(new Chunk(i, j));
			// }
            Chunk* chunk = chunkMap.getChunk(i, j);
            if (chunk->getMeshBuilt() == Unbuilt) {
                chunkQueue.push({(int)i, (int)j});
                chunk->setMeshbuilt(Queued);
            }
        }
    }
    // if (chunkMap.getChunk(chunkX, chunkZ) == nullptr) {
    //     chunkMap.addChunk(new Chunk(chunkX, chunkZ));
    //     chunkQueue.push({(int)chunkX, (int)chunkZ});
        
    // }

    if (!chunkQueue.empty()) {
        ChunkCoord coord = chunkQueue.front();
        // printf("%d, %d\n", coord.x, coord.z);
        Chunk* chunk = chunkMap.getChunk(coord.x, coord.z);
        // chunk->initialize();
        chunkMap.initializeChunkRadius(coord, 1);
        worldMesh.buildMesh(coord);
        chunkQueue.pop();
    }
}