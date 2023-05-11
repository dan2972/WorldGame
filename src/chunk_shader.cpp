#include "chunk_shader.h"

ChunkShader::ChunkShader() {
    m_shader.compile("res/default_shader.vert", "res/default_shader.frag");
}

void ChunkShader::use() {
    m_shader.use();
}

void ChunkShader::loadProjViewMatrix(const glm::mat4& pvmat) {
    m_shader.setMatrix4("projView", pvmat);
}