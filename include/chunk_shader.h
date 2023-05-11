#ifndef CHUNK_SHADER_H
#define CHUNK_SHADER_H

#include "shader.h"

class ChunkShader {
public:
    ChunkShader();

    void use();

    void loadProjViewMatrix(const glm::mat4& pvmat);
private:
    Shader m_shader;
};

#endif