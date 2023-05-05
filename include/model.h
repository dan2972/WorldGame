#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include "mesh.h"

class Model {
public:
    Model() = default;

    void addData(const Mesh& mesh);
    void deleteData();

    void genVAO();
    void addEBO(const std::vector<GLuint> &indices);
    void addVBO(int dimensions, const std::vector<GLfloat> &data);
    void bindVAO() const;

    int getIndicesCount() const;
    int getVAO() const;

private:
    GLuint m_vao;
    GLuint m_indicesCount;

    int m_vboCount = 0;
    std::vector<GLuint> m_buffers;
};
#endif