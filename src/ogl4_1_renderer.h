#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <list>
#include <map>
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
    std::map<Texture, GLuint> textures;
    std::list<std::pair<Mesh, GLuint>> meshes;
    Logger log = Logger("ogl4_1_renderer");

    void drawModels();
    void prepareMesh(Mesh &mesh);
    void loadTextures(Model &model);
    GLuint textureFromFile(
        const std::string filename, bool flipTexture);
    void drawMesh(std::pair<Mesh, GLuint> meshVao);
};
