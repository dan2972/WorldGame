#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include "mesh.h"

struct RenderInfo {
    GLuint vao;
    GLuint indicesCount;
};

class Model {
public:
    Model() = default;

    void addData(const Mesh& mesh);
    void deleteData();

    void genVAO();
    void addEBO(const std::vector<GLuint> &indices);
    void bindVAO() const;

    void setupVBO(const std::vector<int>& dimensions, const std::vector<GLfloat> &data);
    
    const RenderInfo& getRenderInfo() const;

private:
    RenderInfo m_renderInfo;

    int m_vboCount = 0;
    std::vector<GLuint> m_buffers;
};
#endif