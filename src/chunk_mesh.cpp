#include <array>
#include "chunk_mesh.h"
#include "chunk.h"

void ChunkMesh::addFace(const std::array<GLfloat, 12> &blockFace,
                        const std::array<GLfloat, 8> &textureCoords,
                        const glm::vec3 &chunkPosition,
                        const glm::vec3 &blockPosition){
    m_faces++;
    auto &verticies = m_mesh.vertexPositions;
    auto &texCoords = m_mesh.textureCoords;
    auto &indices = m_mesh.indices;

    texCoords.insert(texCoords.end(), textureCoords.begin(),
                     textureCoords.end());
    
    for (int i = 0, index = 0; i < 4; ++i) {
        verticies.push_back(blockFace[index++] + chunkPosition.x * Chunk::CHUNK_SIZE +
                            blockPosition.x);
        verticies.push_back(blockFace[index++] + chunkPosition.y * Chunk::CHUNK_SIZE +
                            blockPosition.y);
        verticies.push_back(blockFace[index++] + chunkPosition.z * Chunk::CHUNK_SIZE +
                            blockPosition.z);
        //m_light.push_back(cardinalLight);
    }

    indices.insert(indices.end(),
                   {m_indexCounter, m_indexCounter + 1, m_indexCounter + 2,

                    m_indexCounter + 2, m_indexCounter + 3, m_indexCounter});
    m_indexCounter += 4;
}

void ChunkMesh::bufferMesh() {
    m_model.addData(m_mesh);
    // m_model.addVBO(1, m_light);

    m_mesh.vertexPositions.clear();
    m_mesh.textureCoords.clear();
    m_mesh.indices.clear();
    // m_light.clear();

    m_mesh.vertexPositions.shrink_to_fit();
    m_mesh.textureCoords.shrink_to_fit();
    m_mesh.indices.shrink_to_fit();
    // m_light.shrink_to_fit();

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