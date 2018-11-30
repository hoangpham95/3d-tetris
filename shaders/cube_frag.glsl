#version 330 core

in vec3 FragPos;

uniform vec4 cubeColor;

out vec4 FragColor;

void main() {
    FragColor = cubeColor;
}
