#include <array>
#include "chunk_mesh.h"
#include "chunk.h"

void ChunkMesh::addFace(const std::array<GLfloat, 12> &blockFace,
                        const std::array<GLfloat, 8> &textureCoords,
                        const glm::vec3 &chunkPosition,
                        const glm::vec3 &blockPosition,
                        const std::array<GLfloat, 4> &cardinalLight, bool flipQuad){
    m_faces++;
    auto &data = m_mesh.data;
    auto &indices = m_mesh.indices;


    for (int i = 0, index = 0, texIndex = 0, lightIndex = 0; i < 4; ++i) {
        data.push_back(blockFace[index++] + chunkPosition.x * CHUNK_SIZE +
                       blockPosition.x);
        data.push_back(blockFace[index++] + chunkPosition.y * CHUNK_SIZE +
                       blockPosition.y);
        data.push_back(blockFace[index++] + chunkPosition.z * CHUNK_SIZE +
                       blockPosition.z);
        data.push_back(textureCoords[texIndex++]);
        data.push_back(textureCoords[texIndex++]);
        data.push_back(cardinalLight[lightIndex++]);
    }

    if (!flipQuad) {
        indices.insert(indices.end(),
                    {m_indexCounter, m_indexCounter + 1, m_indexCounter + 2,

                        m_indexCounter + 2, m_indexCounter + 3, m_indexCounter});
    } else {
        indices.insert(indices.end(),
                    {m_indexCounter + 3, m_indexCounter, m_indexCounter + 1,

                        m_indexCounter + 1, m_indexCounter + 2, m_indexCounter+3});
    }

    m_indexCounter += 4;
}

void ChunkMesh::bufferMesh() {
    m_model.addData(m_mesh);

    m_mesh.data.clear();
    m_mesh.indices.clear();

    m_mesh.data.shrink_to_fit();
    m_mesh.indices.shrink_to_fit();

    m_indexCounter = 0;
}

const Model& ChunkMesh::getModel() const {
    return m_model;
}

void ChunkMesh::deleteData() {
    m_model.deleteData();
}

int ChunkMesh::getNumFaces() {
    return m_faces;
}