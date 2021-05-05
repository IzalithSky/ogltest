#include "ogl4_1_renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_loader.h"

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float FOV   =  90.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float DRAW_RANGE = 1000;

static void error_callback(int error, const char* description) {
    std::cout << "Error: " << error << std::endl << description;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    FOV -= (float)yoffset;
    if (FOV < 1.0f) {
        FOV = 1.0f;
    }
    if (FOV > 120.0f) {
        FOV = 120.0f;
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    // SCR_WIDTH = width;
    // SCR_HEIGHT = height;
}

int ogl4_1_renderer::init() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        return 1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "ogltest window", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwSwapInterval(1);

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    if (!getProgram("vertex_shader.glsl", "fragment_shader.glsl", program)) {
        std::cout << "Failed to load shaders" << std::endl;
        glfwTerminate();
        return 1;
    }

    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    
    model_location = glGetUniformLocation(program, "model");
    view_location = glGetUniformLocation(program, "view");
    projection_location = glGetUniformLocation(program, "projection");

    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vpos_location);
    
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(vertices[0]), (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(vcol_location);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    return 0;
}

bool ogl4_1_renderer::shouldClose() {
    return glfwWindowShouldClose(window);
}

void ogl4_1_renderer::processInput() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void ogl4_1_renderer::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(FOV), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, DRAW_RANGE);
    
    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ogl4_1_renderer::close() {
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);
    glfwDestroyWindow(window);
    glfwTerminate();
}
