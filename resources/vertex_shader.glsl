#version 330 core

in vec3 vPos;
in vec3 vCol;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    color = vCol;
}
