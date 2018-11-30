#version 330 core

// the position of the vertex
layout(location=0)in vec3 position;
// the color of the cube
uniform vec4 color;

uniform mat4 model; // Object space
uniform mat4 view; // Object space
uniform mat4 projection; // Object space

out vec3 FragPos;

void main() {
    
    gl_Position = projection * view * model * vec4(position, 1.0f);
    
    FragPos = vec3(model* vec4(position,1.0f));
}
