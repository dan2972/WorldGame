#include "block_database.h"

std::unordered_map<BlockType, TexCoords> BlockDatabase::m_texCoordMap;

void BlockDatabase::addBlockData(BlockType type, TexPos TexPos) {
    m_texCoordMap.emplace(type, convertPosToTexCoords(TexPos[1], TexPos[0]));
}

std::array<GLfloat, 8> BlockDatabase::getTexCoord(BlockType type) {
    return m_texCoordMap.at(type);
}

TexCoords BlockDatabase::convertPosToTexCoords(unsigned row, unsigned col) {
    row = (ATLAS_HEIGHT / TEXTURE_SIZE) - row - 1;
    GLfloat s = (GLfloat)(TEXTURE_SIZE) / (GLfloat)ATLAS_WIDTH;
    GLfloat y = (GLfloat)(row * TEXTURE_SIZE) / (GLfloat)ATLAS_HEIGHT;
    GLfloat x = (GLfloat)(col * TEXTURE_SIZE) / (GLfloat)ATLAS_WIDTH;
    return {x, y, x+s, y, x+s, y+s, x, y+s};
}