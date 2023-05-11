#ifndef CHUNK_RENDERER_H
#define CHUNK_RENDERER_H

#include "chunk_mesh.h"
#include "camera.h"
#include "chunk_shader.h"
#include <vector>

class ChunkRenderer {
public:
    void addChunk(const ChunkMesh& mesh);
    void render(const Camera &camera);
private:
    std::vector<const RenderInfo*> m_chunkRenderInfos;

    ChunkShader m_shader;
};

#endif