#include "world_mesh_builder.h"
#include "block_database.h"
#include <glm/vec3.hpp>

#define OCCLUSION_INTENSITY 0.15f;

namespace {
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

    // const std::array<GLfloat, 8> texCoords{
    //     0, 0, 1, 0, 1, 1, 0, 1,
    // };

    const std::array<GLfloat, 12> bottomFace{
        0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1
    };

    constexpr GLfloat LIGHT_TOP = 1.0f;
    constexpr GLfloat LIGHT_X = 0.9f;
    constexpr GLfloat LIGHT_Z = 0.8f;
    constexpr GLfloat LIGHT_BOT = 0.7f;

    int vertexAO(bool side1, bool side2, bool corner) {
        if(side1 && side2)
            return 0;
        return 3 - (side1 + side2 + corner);
    }
}

BlockType WorldMeshBuilder::getLeftBlock(int x, int y, int z, const Chunk& chunk) {
    if (x > 0) {
        return chunk.getBlockAt(x-1, y, z);
    } else {
        int worldX = chunk.getChunkX() * CHUNK_SIZE + x;
		int worldZ = chunk.getChunkZ() * CHUNK_SIZE + z;
        return m_chunkMap->getBlockAt(worldX-1, y, worldZ);
    }
}

BlockType WorldMeshBuilder::getRightBlock(int x, int y, int z, const Chunk& chunk) {
    if (x < CHUNK_SIZE - 1) {
        return chunk.getBlockAt(x+1, y, z);
    } else {
        int worldX = chunk.getChunkX() * CHUNK_SIZE + x;
		int worldZ = chunk.getChunkZ() * CHUNK_SIZE + z;
        return m_chunkMap->getBlockAt(worldX+1, y, worldZ);
    }
}

BlockType WorldMeshBuilder::getFrontBlock(int x, int y, int z, const Chunk& chunk) {
    if (z < CHUNK_SIZE - 1) {
        return chunk.getBlockAt(x, y, z+1);
    } else {
        int worldX = chunk.getChunkX() * CHUNK_SIZE + x;
		int worldZ = chunk.getChunkZ() * CHUNK_SIZE + z;
        return m_chunkMap->getBlockAt(worldX, y, worldZ+1);
    }
}

BlockType WorldMeshBuilder::getBackBlock(int x, int y, int z, const Chunk& chunk) {
    if (z > 0) {
        return chunk.getBlockAt(x, y, z-1);
    } else {
        int worldX = chunk.getChunkX() * CHUNK_SIZE + x;
		int worldZ = chunk.getChunkZ() * CHUNK_SIZE + z;
        return m_chunkMap->getBlockAt(worldX, y, worldZ-1);
    }
}

BlockType WorldMeshBuilder::getUpBlock(int x, int y, int z, const Chunk& chunk) {
    if (y < CHUNK_SIZE_Y - 1) {
        return chunk.getBlockAt(x, y+1, z);
    } else {
        return Air;
    }
}

BlockType WorldMeshBuilder::getDownBlock(int x, int y, int z, const Chunk& chunk) {
    if (y > 0) {
        return chunk.getBlockAt(x, y-1, z);
    } else {
        return Air;
    }
}

WorldMeshBuilder::WorldMeshBuilder(ChunkMap* chunkMap, ChunkRenderer* chunkRenderer) {
    m_chunkMap = chunkMap;
    m_chunkRenderer = chunkRenderer;
}

void WorldMeshBuilder::buildMeshes() {
    for (auto chunk : m_chunkMap->getChunks()) {
        printf("generating chunk: %d, %d\n", chunk->getChunkX(), chunk->getChunkZ());
        buildChunkMesh(*chunk);
    }
}

void WorldMeshBuilder::render(const Camera& camera) {
    for (auto& it : m_chunkMeshes) {
        m_chunkRenderer->addChunk(*(it.second.get()));
    }
    m_chunkRenderer->render(camera);
}

ChunkMesh* WorldMeshBuilder::getChunkMesh(ChunkCoord pos) {
    if (m_chunkMeshes.find(pos) == m_chunkMeshes.end())
        return nullptr;
    return m_chunkMeshes.at(pos).get();
}

std::vector<ChunkMesh*> WorldMeshBuilder::getChunkMeshes() {
    std::vector<ChunkMesh*> meshes;
    for (auto& it : m_chunkMeshes) {
        meshes.push_back(it.second.get());
    }
    return meshes;
}

void WorldMeshBuilder::buildChunkMesh(const Chunk& chunk) {
    ChunkMesh* mesh = new ChunkMesh();
    const glm::vec3 chunkPos{chunk.getChunkX(), 0, chunk.getChunkZ()};
    for (int y = 0; y < CHUNK_SIZE_Y; ++y) {
		for (int z = 0; z < CHUNK_SIZE; ++z) {
			for (int x = 0; x < CHUNK_SIZE; ++x) {
                BlockType bType = chunk.getBlockAt(x, y, z);
                if (bType != Air) {
                    const std::array<GLfloat, 8> texCoords = BlockDatabase::getTexCoord(bType);
                    int worldX = chunk.getChunkX() * CHUNK_SIZE + x;
		            int worldZ = chunk.getChunkZ() * CHUNK_SIZE + z;
                    if(getLeftBlock(x, y, z, chunk) == Air) {
                        bool left = m_chunkMap->getBlockAt(worldX-1, y, worldZ+1) != Air;
                        bool right = m_chunkMap->getBlockAt(worldX-1, y, worldZ-1) != Air;
                        bool up = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ) != Air;
                        bool down = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ) != Air;
                        bool leftup = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ+1) != Air;
                        bool leftdown = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ+1) != Air;
                        bool rightup = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ-1) != Air;
                        bool rightdown = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ-1) != Air;

                        int a0 = vertexAO(left, down, leftdown);
                        int a1 = vertexAO(down, right, rightdown);
                        int a2 = vertexAO(right, up, rightup);
                        int a3 = vertexAO(up, left, leftup);

                        float ld = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float rd = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float ru = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float lu = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a0+a2 > a1+a3;
                        
                        mesh->addFace(leftFace, texCoords, chunkPos, {x, y, z}, {LIGHT_X-rd, LIGHT_X-ld, LIGHT_X-lu, LIGHT_X-ru}, flip);
                    }
                    if(getRightBlock(x, y, z, chunk) == Air) {

                        bool right = m_chunkMap->getBlockAt(worldX+1, y, worldZ+1) != Air;
                        bool left = m_chunkMap->getBlockAt(worldX+1, y, worldZ-1) != Air;
                        bool up = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ) != Air;
                        bool down = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ) != Air;
                        bool rightup = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ+1) != Air;
                        bool rightdown = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ+1) != Air;
                        bool leftup = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ-1) != Air;
                        bool leftdown = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ-1) != Air;

                        int a0 = vertexAO(right, down, rightdown);
                        int a1 = vertexAO(down, left, leftdown);
                        int a2 = vertexAO(left, up, leftup);
                        int a3 = vertexAO(up, right, rightup);

                        float rd = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float ld = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float lu = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float ru = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a0+a2 > a1+a3;

                        mesh->addFace(rightFace, texCoords, chunkPos, {x, y, z}, {LIGHT_X-rd, LIGHT_X-ld, LIGHT_X-lu, LIGHT_X-ru}, flip);
                    }
                    if(getFrontBlock(x, y, z, chunk) == Air) {

                        bool left = m_chunkMap->getBlockAt(worldX-1, y, worldZ+1) != Air;
                        bool right = m_chunkMap->getBlockAt(worldX+1, y, worldZ+1) != Air;
                        bool up = m_chunkMap->getBlockAt(worldX, y+1, worldZ+1) != Air;
                        bool down = m_chunkMap->getBlockAt(worldX, y-1, worldZ+1) != Air;
                        bool leftup = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ+1) != Air;
                        bool leftdown = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ+1) != Air;
                        bool rightup = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ+1) != Air;
                        bool rightdown = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ+1) != Air;

                        int a0 = vertexAO(left, down, leftdown);
                        int a1 = vertexAO(down, right, rightdown);
                        int a2 = vertexAO(right, up, rightup);
                        int a3 = vertexAO(up, left, leftup);

                        float ld = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float rd = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float ru = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float lu = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a0+a2 < a1+a3;

                        mesh->addFace(frontFace, texCoords, chunkPos, {x, y, z}, {LIGHT_Z-ld, LIGHT_Z-rd, LIGHT_Z-ru, LIGHT_Z-lu}, flip);
                    }
                    if(getBackBlock(x, y, z, chunk) == Air) {

                        bool left = m_chunkMap->getBlockAt(worldX-1, y, worldZ-1) != Air;
                        bool right = m_chunkMap->getBlockAt(worldX+1, y, worldZ-1) != Air;
                        bool up = m_chunkMap->getBlockAt(worldX, y+1, worldZ-1) != Air;
                        bool down = m_chunkMap->getBlockAt(worldX, y-1, worldZ-1) != Air;
                        bool leftup = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ-1) != Air;
                        bool leftdown = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ-1) != Air;
                        bool rightup = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ-1) != Air;
                        bool rightdown = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ-1) != Air;

                        int a0 = vertexAO(left, down, leftdown);
                        int a1 = vertexAO(down, right, rightdown);
                        int a2 = vertexAO(right, up, rightup);
                        int a3 = vertexAO(up, left, leftup);

                        float ld = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float rd = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float ru = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float lu = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a0+a2 > a1+a3;

                        mesh->addFace(backFace, texCoords, chunkPos, {x, y, z}, {LIGHT_Z-rd, LIGHT_Z-ld, LIGHT_Z-lu, LIGHT_Z-ru}, flip);
                    }
                    if(getUpBlock(x, y, z, chunk) == Air) {
                        
                        bool left = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ) != Air;
                        bool right = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ) != Air;
                        bool front = m_chunkMap->getBlockAt(worldX, y+1, worldZ+1) != Air;
                        bool back = m_chunkMap->getBlockAt(worldX, y+1, worldZ-1) != Air;
                        bool leftfront = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ+1) != Air;
                        bool rightfront = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ+1) != Air;
                        bool leftback = m_chunkMap->getBlockAt(worldX-1, y+1, worldZ-1) != Air;
                        bool rightback = m_chunkMap->getBlockAt(worldX+1, y+1, worldZ-1) != Air;

                        int a0 = vertexAO(left, front, leftfront);
                        int a1 = vertexAO(front, right, rightfront);
                        int a2 = vertexAO(right, back, rightback);
                        int a3 = vertexAO(back, left, leftback);

                        float lf = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float rf = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float rb = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float lb = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a3+a1 > a0+a2;

                        mesh->addFace(topFace, texCoords, chunkPos, {x, y, z}, {LIGHT_TOP-lf, LIGHT_TOP-rf, LIGHT_TOP-rb, LIGHT_TOP-lb}, flip);
                    }
                    if(getDownBlock(x, y, z, chunk) == Air) {

                        bool left = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ) != Air;
                        bool right = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ) != Air;
                        bool front = m_chunkMap->getBlockAt(worldX, y-1, worldZ+1) != Air;
                        bool back = m_chunkMap->getBlockAt(worldX, y-1, worldZ-1) != Air;
                        bool leftfront = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ+1) != Air;
                        bool rightfront = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ+1) != Air;
                        bool leftback = m_chunkMap->getBlockAt(worldX-1, y-1, worldZ-1) != Air;
                        bool rightback = m_chunkMap->getBlockAt(worldX+1, y-1, worldZ-1) != Air;

                        int a0 = vertexAO(left, front, leftfront);
                        int a1 = vertexAO(front, right, rightfront);
                        int a2 = vertexAO(right, back, rightback);
                        int a3 = vertexAO(back, left, leftback);

                        float lf = (float)(3-a0)*OCCLUSION_INTENSITY;
                        float rf = (float)(3-a1)*OCCLUSION_INTENSITY;
                        float rb = (float)(3-a2)*OCCLUSION_INTENSITY;
                        float lb = (float)(3-a3)*OCCLUSION_INTENSITY;

                        bool flip = a3+a1 < a0+a2;

                        mesh->addFace(bottomFace, texCoords, chunkPos, {x, y, z}, {LIGHT_BOT-lb, LIGHT_BOT-rb, LIGHT_BOT-rf, LIGHT_BOT-lf}, flip);
                    }
                }
            }
        }
    }
    mesh->bufferMesh();
    m_chunkMeshes.emplace(ChunkCoord{chunk.getChunkX(), chunk.getChunkZ()}, std::make_unique<ChunkMesh>(*mesh));
}