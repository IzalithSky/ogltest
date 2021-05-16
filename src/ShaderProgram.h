#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <glad/glad.h>
#include <exception>

class ShaderProgram {
public:
    GLuint programId;

    GLuint position;
    GLuint textureCoords;
    GLint model_location;
    GLint view_location;
    GLint projection_location;

    ShaderProgram(std::string vertexPath, std::string fragmentPath);
    
    void use();
};

#endif
