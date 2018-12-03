
#version 330 core
in vec4 FragColor;
out vec4 color;

//in vec3 theColor;

void main()
{
    // color is a vec4 representing color. Because we are in a fragment
    // shader, we are expecting in our pipeline a color output.
    // That is essentially the job of the fragment shader!
    color = FragColor;
}

