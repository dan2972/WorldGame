#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <glm/vec3.hpp>
#include "model.h"
#include "mesh.h"

class ChunkMesh {
public:
    ChunkMesh() = default;

    void addFace(const std::array<GLfloat, 12> &blockFace,
                 const std::array<GLfloat, 8> &textureCoords,
                 const glm::vec3 &chunkPosition,
                 const glm::vec3 &blockPosition);

    void bufferMesh();

    const Model &getModel() const;

    void deleteData();

    int getNumFaces();
private:
    Mesh m_mesh;
    Model m_model;

    int m_faces = 0;
    GLuint m_indexCounter = 0;
};

#endif