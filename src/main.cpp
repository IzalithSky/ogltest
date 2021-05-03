#include <iostream>
#include "ogl4_1_renderer.h"

int main() {
    ogl4_1_renderer renderer;
    if (renderer.init()) {
        std::cout << "Initialisation failed" << std::endl;
    }
    while (!renderer.shouldClose()) {
        renderer.draw();
    }
    renderer.close();
    return 0;
}
