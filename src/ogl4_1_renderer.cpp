#include "ogl4_1_renderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in 
// a direction vector pointing to the right so we initially
// rotate a bit to the left.
float yaw   = -90.0f; 
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float FOV   =  90.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float DRAW_RANGE = 1024;

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
    // reversed since y-coordinates go from bottom to top
    float yoffset = lastY - ypos;
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

static void framebuffer_size_callback(
    GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; 
    // note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
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
    
    if (vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    try {
        program = new ShaderProgram(
            "vertex_shader.glsl", "fragment_shader.glsl");
    } catch (std::exception) {
        std::cout << "Failed to load shaders" << std::endl;
        glfwTerminate();
        return 1;
    }
    
    program->use();

    return 0;
}

void ogl4_1_renderer::addAsset(Asset asset) {
    assets.push_back(asset);
    for (GLuint i = 0; i < asset.model.meshes.size(); i++) {
        prepareMesh(asset.model.meshes[i]);
    }
    loadTextures(asset.model);
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) 
            * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) 
            * cameraSpeed;
    }
}

void ogl4_1_renderer::draw() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(
        glm::radians(FOV), (float) SCR_WIDTH / SCR_HEIGHT, 0.1f, DRAW_RANGE);
    
    glUniformMatrix4fv(
        program->view_location, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(
        program->projection_location, 1, GL_FALSE, glm::value_ptr(projection));

    drawModels();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void ogl4_1_renderer::drawModels() {
    std::list<Asset>::iterator it;
    // make sure to initialize matrix to identity matrix first
    glm::mat4 modelMat = glm::mat4(1.0f);

    float f = 1;
    for (it = assets.begin(); it != assets.end(); ++it) {
        glm::mat4 pos = glm::translate(modelMat, it->pos);
        // pos = glm::rotate(
        //     pos, (float) glfwGetTime() * glm::radians(50.0f * f), 
        //     glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(
            program->model_location, 1, GL_FALSE, glm::value_ptr(pos));

        for (std::pair<Mesh, GLuint> p : meshes) {
            drawMesh(p);
        }

        f += .1;
    }
}

void ogl4_1_renderer::loadTextures(Model &model) {
    for(Texture texture : model.texturesInfo) {
        if (textures.find(texture) == textures.end()) {
            textures[texture] = textureFromFile(
                texture.path, texture.flipped);
        }
    }
}

GLuint ogl4_1_renderer::textureFromFile(
    const std::string filename, bool flipTexture) {

    log.info("loading texture: " + filename);

    GLuint textureID;
    glGenTextures(1, &textureID);
    log.info("textureID: " + std::to_string(textureID));

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flipTexture);
    unsigned char *data = 
        stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1) {
            log.info("format GL_RED");
            format = GL_RED;
        } else if (nrComponents == 3) {
            log.info("format GL_RGB");
            format = GL_RGB;
        } else if (nrComponents == 4) {
            log.info("format GL_RGBA");
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(
            GL_TEXTURE_2D, 0, format, width, height, 0, format, 
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        log.error("texture failed to load at path: " + filename);
        stbi_image_free(data);
    }

    return textureID;
}

void ogl4_1_renderer::prepareMesh(Mesh &mesh) {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    
    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    log.info(
        "prepering mesh: VAO " + std::to_string(VAO)
        + ", VBO " + std::to_string(VBO)
        + ", EBO " + std::to_string(EBO));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), 
        &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint), 
        &mesh.indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(program->position);
    glVertexAttribPointer(
        program->position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(program->textureCoords);
    glVertexAttribPointer(
        program->textureCoords, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

    meshes.push_back(std::make_pair(mesh, VAO));
}

void ogl4_1_renderer::drawMesh(std::pair<Mesh, GLuint> meshVao) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    GLuint normalNr = 1;
    GLuint heightNr = 1;
    for (GLuint i = 0; i < meshVao.first.textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); 
        // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = meshVao.first.textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        } else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
        } else if (name == "texture_normal") {
            number = std::to_string(normalNr++);
        } else if (name == "texture_height") {
            number = std::to_string(heightNr++);
        }

        // now set the sampler to the correct texture unit
        glUniform1i(
            glGetUniformLocation(
                program->programId, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[meshVao.first.textures[i]]);
    }
    
    GLuint VAO = meshVao.second;
    glBindVertexArray(VAO);
    glDrawElements(
        GL_TRIANGLES, meshVao.first.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void ogl4_1_renderer::close() {
    glfwDestroyWindow(window);
    glfwTerminate();
}
