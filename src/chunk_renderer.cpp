#include "chunk_renderer.h"
#include "resource_manager.h"

void ChunkRenderer::addChunk(const ChunkMesh& mesh) {
    m_chunkRenderInfos.push_back(&mesh.getModel().getRenderInfo());
}

void ChunkRenderer::render(const Camera &camera) {
    if (m_chunkRenderInfos.empty()) {
        return;
    }

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader.use();
    Texture2D texture = ResourceManager::getTexture("atlas");
	texture.bind();
    
    m_shader.loadProjViewMatrix(camera.getProjViewMatrix());

    for (auto mesh : m_chunkRenderInfos) {
        glBindVertexArray(mesh->vao);
        glDrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);
    }

    m_chunkRenderInfos.clear();
}