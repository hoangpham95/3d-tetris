
#version 330 core
in vec4 FragColor;
in vec3 OriginPosition;

uniform float cubeHalfsize; // use to calculate normal

out vec4 color;

//in vec3 theColor;

void main()
{
    bvec3 surface = equal(vec3(cubeHalfsize, cubeHalfsize, cubeHalfsize), abs(OriginPosition));
    float r = 0;
    float g = 0;
    float b = 0;
    if(surface.x) {
        r = 1;
    }
    if(surface.y) {
        g = 1;
    }
    if(surface.z) {
        b = 1;
    }
    
//    vec3 normal = dot(sign(OriginPosition),surface);
    // color is a vec4 representing color. Because we are in a fragment
    // shader, we are expecting in our pipeline a color output.
    // That is essentially the job of the fragment shader!
//    color = FragColor;
    color = vec4(r,g,b,1);
}

