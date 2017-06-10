#version 420

in vec3 vPos;
in vec2 vUV;
in vec3 vNorm;
flat in int vMaterialIndex;

out vec4 outColour;
out vec4 outNormal;
out vec4 outPosition;

uniform sampler2DArray textureSampler;

void main()
{
    float intensity = dot(normalize(vec3(1,1,1)), vNorm);
    vec3 colour = texture(textureSampler, vec3(vUV, vMaterialIndex)).rgb;
    //colour *= intensity;

    outColour = vec4(colour,1);
    outNormal = vec4(vNorm,1);
    outPosition = vec4(vPos,1);
}
