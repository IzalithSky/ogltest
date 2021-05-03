#include "ogl4_1_renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_loader.h"

static void error_callback(int error, const char* description) {
    std::cout << "Error: " << error << std::endl << description;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    glfwSetKeyCallback(window, key_callback);

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

void ogl4_1_renderer::draw() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 0.5f));
        
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); // note that we're translating the scene in the reverse direction of where we want to move

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
