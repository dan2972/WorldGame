#ifndef CHUNKMESH_H
#define CHUNKMESH_H

#include <glm/vec3.hpp>
#include <array>
#include "model.h"
#include "mesh.h"

class ChunkMesh {
public:
    ChunkMesh() = default;

    void addFace(const std::array<GLfloat, 12> &blockFace,
                 const std::array<GLfloat, 12> &textureCoords,
                 const glm::vec3 &chunkPosition,
                 const glm::vec3 &blockPosition,
                 const std::array<GLfloat, 4> &cardinalLight, bool flipQuad = false);

    void bufferMesh();

    const Model &getModel() const;

    void deleteData();

    int getNumFaces();
    Model m_model;
private:
    Mesh m_mesh;
    std::vector<GLfloat> m_light;
    int m_faces = 0;
    GLuint m_indexCounter = 0;
};

#endif