#pragma once

#include <vector>
#include "chunk.h"
#include "chunk_map.h"
#include "chunk_mesh.h"
#include "camera.h"
#include "chunk_renderer.h"

typedef std::unordered_map<ChunkCoord, std::unique_ptr<ChunkMesh>, HashFunc, EqualsFunc> ChunkMeshMap;

class WorldMeshBuilder{
public:
    WorldMeshBuilder(ChunkMap* chunkMap, ChunkRenderer* chunkRenderer);

    void buildMeshes();
    void buildMesh(ChunkCoord pos);

    void render(const Camera& camera);

    ChunkMesh* getChunkMesh(ChunkCoord pos);
    std::vector<ChunkMesh*> getChunkMeshes();
private:
    void buildChunkMesh(const Chunk& chunk);

    BlockType getLeftBlock(int x, int y, int z, const Chunk& chunk);
    BlockType getRightBlock(int x, int y, int z, const Chunk& chunk);
    BlockType getFrontBlock(int x, int y, int z, const Chunk& chunk);
    BlockType getBackBlock(int x, int y, int z, const Chunk& chunk);
    BlockType getUpBlock(int x, int y, int z, const Chunk& chunk);
    BlockType getDownBlock(int x, int y, int z, const Chunk& chunk);

    ChunkMap* m_chunkMap = nullptr;
    ChunkRenderer* m_chunkRenderer = nullptr;
    ChunkMeshMap m_chunkMeshes;
};