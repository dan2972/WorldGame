#include "model.h"

void Model::addData(const Mesh& mesh) {
    genVAO();

    addVBO(3, mesh.vertexPositions);
    addVBO(2, mesh.textureCoords);
    addEBO(mesh.indices);
}

void Model::deleteData() {
    if (m_vao)
        glDeleteVertexArrays(1, &m_vao);
    if (m_buffers.size() > 0)
        glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()),
                        m_buffers.data());

    m_buffers.clear();

    m_vboCount = 0;
    m_vao = 0;
    m_indicesCount = 0;
}

void Model::genVAO() {
    if (m_vao != 0)
        deleteData();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
}

void Model::addEBO(const std::vector<GLuint> &indices) {
    m_indicesCount = static_cast<GLuint>(indices.size());
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);
}

void Model::addVBO(int dimensions, const std::vector<GLfloat> &data){
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(static_cast<GLuint>(m_vboCount), dimensions, GL_FLOAT,
                          GL_FALSE, 0, (GLvoid *)0);

    glEnableVertexAttribArray(static_cast<GLuint>(m_vboCount++));

    m_buffers.push_back(vbo);
}

void Model::bindVAO() const{
    glBindVertexArray(m_vao);
}

int Model::getIndicesCount() const{
    return m_indicesCount;
}

int Model::getVAO() const{
    return m_vao;
}