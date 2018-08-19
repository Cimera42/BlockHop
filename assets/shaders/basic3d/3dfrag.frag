#version 420

layout(location=0) in vec3 inPos;
layout(location=1) in vec2 inUV;
layout(location=2) in vec3 inNorm;
layout(location=3) flat in int inMaterialIndex;

out vec4 outColour;
out vec4 outNormal;
out vec4 outPosition;

uniform sampler2DArray textureSampler;

void main()
{
    float intensity = clamp(dot(normalize(vec3(1,1,1)), inNorm), 0,1);
    vec3 colour = texture(textureSampler, vec3(inUV, inMaterialIndex)).rgb;
    colour *= intensity;

    outColour = vec4(colour,1);
    outNormal = vec4(inNorm,1);
    outPosition = vec4(inPos,1);
}
