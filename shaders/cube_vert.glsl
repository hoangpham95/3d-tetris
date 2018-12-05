// ==================================================================
#version 330 core

layout(location=0)in vec3 position; // the position of the vertex in the tetrix world
layout(location=1)in vec4 vertexColor; // Our second attribute which is the color.
layout(location=2)in vec3 originPosition; // the position of the vertex if the center of the cube is at the origin, used to calculate normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// Do not forget this!
out vec4 FragColor;
out vec3 OriginPosition;

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
    OriginPosition = originPosition;
    
}
// ==================================================================
