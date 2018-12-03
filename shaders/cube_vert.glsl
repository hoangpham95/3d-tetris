// ==================================================================
#version 330 core

layout(location=0)in vec3 position; // We explicitly state which is the vertex information (The first 3 floats are positional data, we are putting in our vector)
layout(location=1)in vec4 vertexColor; // Our second attribute which is the color.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Do not forget this!
out vec4 FragColor;

void main()
{
    // gl_Position is a special glsl variable that tells us what
    // postiion to put things in.
    // It takes in exactly 4 things.
    // Note that 'w' (the 4th dimension) should be 1.
    gl_Position = projection * view * model * vec4(position, 1.0f);
    
    // Store the vertex color that we take in as what we will output
    // to the next stage in the graphics pipeline.
    FragColor = vertexColor;
    
}
// ==================================================================
