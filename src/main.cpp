#include <iostream>
#include "ogl4_1_renderer.h"
#include "model.h"
#include "filesystem.h"

int main() {
    Logger log;

    log.info("hi");

    ogl4_1_renderer renderer;
    renderer.vsync = false;
    if (renderer.init()) {
        std::cout << "Initialisation failed" << std::endl;
    }
    
    int count = 1;
    float offset = 4;
    std::cout << "Loading:" << std::endl;

    Model model(FileSystem::getPath("resources/backpack/backpack.obj"));

    // Model model(
    //     FileSystem::getPath("resources/Content/boblampclean.md5mesh"), false);

    for (int i = 1; i <= count; i++) {
        glm::vec3 pos(0.f, 0.f, - offset * i);
        Asset asset = {model, pos};
        renderer.addAsset(asset);

        std::cout << 100 * ((float) i / count) << "%" << std::endl;
    }
    double duration = 0;
    int draws = 0;
    while (!renderer.shouldClose()) {
        double start = glfwGetTime();
        
        renderer.processInput();
        renderer.draw();

        double frame = glfwGetTime() - start;
        duration += frame;
        draws++;
        if (duration >= 1) {
            duration = 0;
            std::cout << draws << ", " << frame << std::endl;
            draws = 0;
        }
    }
    renderer.close();
    return 0;
}
