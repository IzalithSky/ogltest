cmake_minimum_required(VERSION 3.10)

project(ogltest VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(deps/glad)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(src)

include_directories(deps/glad/include)
include_directories(deps/glfw/include)
include_directories(deps/glfw/deps)
include_directories(deps/glm)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

configure_file(
	"${PROJECT_SOURCE_DIR}/resources/vertex_shader.glsl"
	"${PROJECT_BINARY_DIR}/vertex_shader.glsl"
    COPYONLY)

configure_file(
	"${PROJECT_SOURCE_DIR}/resources/fragment_shader.glsl"
	"${PROJECT_BINARY_DIR}/fragment_shader.glsl"
    COPYONLY)

set(SOURCES src/main.cpp src/shader_loader.cpp src/ogl4_1_renderer.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
