#pragma once
#include <array>
#include <unordered_map>
#include <glad/glad.h>
#include "block_type.h"

typedef std::array<GLfloat, 8> TexCoords;
typedef std::array<unsigned, 2> TexPos;

class BlockDatabase {
public:
    static void addBlockData(BlockType type, TexPos TexPos);
    static std::array<GLfloat, 8> getTexCoord(BlockType type);
private:
    static const unsigned ATLAS_WIDTH = 64;
    static const unsigned ATLAS_HEIGHT = 64;
    static const unsigned TEXTURE_SIZE = 16;
    BlockDatabase() {}

    static TexCoords convertPosToTexCoords(unsigned row, unsigned col);

    static std::unordered_map<BlockType, TexCoords> m_texCoordMap;
};