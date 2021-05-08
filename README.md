OpenGL 4 Rendering Test

Dependencies:

    GLWF (https://github.com/glfw/glfw)
    glad (https://github.com/Dav1dde/glad)
    glm (https://github.com/g-truc/glm)
    assimp (https://github.com/assimp/assimp.git)
    stb (https://github.com/nothings/stb)

Build:

    pull dependencies:
        git submodule update --init --recursive

    test model is not included into the repo, download from https://github.com/JoeyDeVries/LearnOpenGL/tree/master/resources/objects/backpack and add under resources/backback (path is hardcoded).

    static build for gcc:        
        set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

Thanks to:

    https://learnopengl.com/
