#version 420

layout(location=0) in vec3 inNorm;

layout(location = 0) out vec4 outColour;

void main()
{
    float intensity = clamp(dot(normalize(vec3(1,1,1)), inNorm), 0,1);
    outColour = vec4(vec3(intensity), 1);
}