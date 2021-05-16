cmake_minimum_required(VERSION 3.10)

project(ogltest VERSION 0.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(deps/glad)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/assimp)
add_subdirectory(src)

include_directories(deps/glad/include)
include_directories(deps/glfw/include)
include_directories(deps/assimp/include)
include_directories(deps/stb)
include_directories(deps/glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

configure_file(
	"${PROJECT_SOURCE_DIR}/src/shaders/vertex_shader.glsl"
	"${PROJECT_BINARY_DIR}/vertex_shader.glsl"
    COPYONLY)

configure_file(
	"${PROJECT_SOURCE_DIR}/src/shaders/fragment_shader.glsl"
	"${PROJECT_BINARY_DIR}/fragment_shader.glsl"
    COPYONLY)

configure_file(root_directory.h.in root_directory.h)
include_directories(${CMAKE_BINARY_DIR})

set(SOURCES src/main.cpp src/ShaderProgram.cpp src/ogl4_1_renderer.cpp src/mesh.cpp src/model.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} assimp)
