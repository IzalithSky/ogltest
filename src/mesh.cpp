#include "mesh.h"

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<GLuint> indices,
    std::vector<Texture> textures) {

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
}
