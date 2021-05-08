#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include "ShaderProgram.h"
#include "model.h"

class ogl4_1_renderer {
public:
    ShaderProgram* program;

    std::list<Model> models;

    int init();
    bool shouldClose();
    void processInput();
    void draw();
    void close();

private:
    GLFWwindow* window;

    void drawModels();
};
