#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include "ShaderProgram.h"
#include "model.h"

struct Asset {
    Model model;
    glm::vec3 pos;
};

class ogl4_1_renderer {
public:
    ShaderProgram* program;
    bool vsync = false;

    int init();
    void addAsset(Asset asset);
    bool shouldClose();
    void processInput();
    void draw();
    void close();
private:
    GLFWwindow* window;
    std::list<Asset> assets;

    void drawModels();
};
