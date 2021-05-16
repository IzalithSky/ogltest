#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

int load_shader(
    GLuint &shader,
    GLenum shader_type,
    std::string shader_text,
    std::string error_str) {
    
    int  success;
    char infoLog[512];
    
    shader = glCreateShader(shader_type);
    const char *c_str = shader_text.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << error_str << infoLog << std::endl;
    }
    return success;
}

bool load_vertex_shader(GLuint &shader, const char* shader_text) {
    int success = load_shader(
        shader,
        GL_VERTEX_SHADER,
        shader_text,
        "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
    return success ? true : false;
}

bool load_fragment_shader(GLuint &shader, const char* shader_text) {
    int success = load_shader(
        shader,
        GL_FRAGMENT_SHADER,
        shader_text,
        "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
    return success ? true : false;
}

bool load_program(GLuint &programId, GLuint vertex_shader, GLuint fragment_shader) {
    programId = glCreateProgram();
    glAttachShader(programId, vertex_shader);
    glAttachShader(programId, fragment_shader);
    glLinkProgram(programId);

    int isLinked;
    glGetShaderiv(programId, GL_LINK_STATUS, &isLinked);
    if (GL_FALSE == isLinked) {
        GLint maxLength = 0;
        char infoLog[0];
        glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
    }
    
    return isLinked ? true : false;
}

bool makeProgramm(
    GLuint &programId,
    const char* vertex_shader_src,
    const char* fragment_shader_src) {

    GLuint vertex_shader, fragment_shader;
    
    if (!load_vertex_shader(vertex_shader, vertex_shader_src)) {
        return false;
    }
    if (!load_fragment_shader(fragment_shader, fragment_shader_src)) {
        return false;
    }
    if (!load_program(programId, vertex_shader, fragment_shader)) {
        return false;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader); 
    
    return true;
}

ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();			
        // if geometry shader path is present, also load a geometry shader
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        throw std::exception();
    }
    
    if (!makeProgramm(programId, vertexCode.c_str(), fragmentCode.c_str())) {
        throw std::exception();
    }

    position = glGetAttribLocation(programId, "position");
    textureCoords = glGetAttribLocation(programId, "textureCoords");

    model_location = glGetUniformLocation(programId, "model");
    view_location = glGetUniformLocation(programId, "view");
    projection_location = glGetUniformLocation(programId, "projection");

    glEnable(GL_DEPTH_TEST);
}

void ShaderProgram::use() {
    glUseProgram(programId);
}
