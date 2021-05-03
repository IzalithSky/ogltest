#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const struct {
    float x, y, z;
    float r, g, b;
} vertices[6] = {
    { -0.6f - 1, -0.4f, 0.0f, 1.f, 0.f, 0.f },
    {  0.6f - 1, -0.4f, 0.0f, 0.f, 1.f, 0.f },
    {   0.f - 1,  0.6f, 0.0f, 0.f, 0.f, 1.f },

    { -0.6f + 1, -0.4f, 0.0f, 1.f, 0.f, 0.f },
    {  0.6f + 1, -0.4f, 0.0f, 0.f, 1.f, 0.f },
    {   0.f + 1,  0.6f, 0.0f, 0.f, 0.f, 1.f }
};

static const unsigned int indices[] = {
    0, 1, 2,
    3, 4, 5
};

class ogl4_1_renderer {
public:
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;
    float FOV = 110;
    float DRAW_RANGE = 1000;

    int init();
    bool shouldClose();
    void draw();
    void close();

private:
    GLFWwindow* window;

    GLuint vertex_array;
    GLuint vertex_buffer;
    GLuint element_buffer;

    GLuint program;

    GLint vpos_location;
    GLint vcol_location;
    
    GLint model_location;
    GLint view_location;
    GLint projection_location;
};
