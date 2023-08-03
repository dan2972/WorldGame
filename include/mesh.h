#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>

struct Mesh{
    std::vector<GLfloat> data;
    std::vector<GLuint> indices;
};

#endif