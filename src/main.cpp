#include <iostream>
#include "ogl4_1_renderer.h"
#include "model.h"
#include "filesystem.h"

int main() {
    ogl4_1_renderer renderer;
    if (renderer.init()) {
        std::cout << "Initialisation failed" << std::endl;
    }
    Model model(FileSystem::getPath("resources/backpack/backpack.obj"));
    renderer.models.push_back(model);
    while (!renderer.shouldClose()) {
        renderer.processInput();
        renderer.draw();
    }
    renderer.close();
    return 0;
}
