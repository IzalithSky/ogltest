#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    std::string path;
    std::string type;
    bool flipped;

    bool operator < (const Texture& rhs) const {       
        return type + path < rhs.type + rhs.path;
    }
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(
        std::vector<Vertex> vertices,
        std::vector<GLuint> indices,
        std::vector<Texture> textures);
};

#endif
