#version 420

in vec3 gPos;
in vec2 gUV;
in vec3 gNorm;
flat in int gMaterialIndex;
in vec3 gColour;

out vec4 outColour;
out vec4 outNormal;
out vec4 outPosition;

uniform sampler2DArray textureSampler;

void main()
{
    float intensity = dot(normalize(vec3(1,1,1)), gNorm);
    vec3 colour = texture(textureSampler, vec3(gUV, gMaterialIndex)).rgb;
    colour *= intensity;

    outColour = vec4(colour,1);
    outNormal = vec4(gNorm,1);
    outPosition = vec4(gPos,1);
}
