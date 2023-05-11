#include "world_mesh_builder.h"
#include <glm/vec3.hpp>

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

    const std::array<GLfloat, 8> texCoords{
        0, 0, 1, 0, 1, 1, 0, 1,
    };

    const std::array<GLfloat, 12> bottomFace{
        0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1
    };

    constexpr GLfloat LIGHT_TOP = 1.0f;
    constexpr GLfloat LIGHT_X = 0.8f;
    constexpr GLfloat LIGHT_Z = 0.6f;
    constexpr GLfloat LIGHT_BOT = 0.4f;
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
    if (y < CHUNK_SIZE - 1) {
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
        printf("found chunk: %d, %d\n", chunk->getChunkX(), chunk->getChunkZ());
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
    for (int y = 0; y < CHUNK_SIZE; ++y) {
		for (int z = 0; z < CHUNK_SIZE; ++z) {
			for (int x = 0; x < CHUNK_SIZE; ++x) {
                if (chunk.getBlockAt(x, y, z) != Air) {
                    if(getLeftBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(leftFace, texCoords, chunkPos, {x, y, z}, LIGHT_X);
                    }
                    if(getRightBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(rightFace, texCoords, chunkPos, {x, y, z}, LIGHT_X);
                    }
                    if(getFrontBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(frontFace, texCoords, chunkPos, {x, y, z}, LIGHT_Z);
                    }
                    if(getBackBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(backFace, texCoords, chunkPos, {x, y, z}, LIGHT_Z);
                    }
                    if(getUpBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(topFace, texCoords, chunkPos, {x, y, z}, LIGHT_TOP);
                    }
                    if(getDownBlock(x, y, z, chunk) == Air) {
                        mesh->addFace(bottomFace, texCoords, chunkPos, {x, y, z}, LIGHT_BOT);
                    }
                }
            }
        }
    }
    mesh->bufferMesh();
    m_chunkMeshes.emplace(ChunkCoord{chunk.getChunkX(), chunk.getChunkZ()}, std::make_unique<ChunkMesh>(*mesh));
}