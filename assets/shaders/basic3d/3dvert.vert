#version 420

layout(location=0) in vec3 vertPos;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNorm;
layout(location=3) in int vertMaterialIndex;

layout(location=0) out vec3 outPos;
layout(location=1) out vec2 outUV;
layout(location=2) out vec3 outNorm;
layout(location=3) flat out int outMaterialIndex;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    outPos = vec3(modelMat * vec4(vertPos,1));
    outUV = vertUV;
    outMaterialIndex = vertMaterialIndex;

    mat3 normalMatrix = mat3(modelMat);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    outNorm = normalize(normalMatrix * vertNorm);
    vec3 a = vec3(0);
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos+a,1);
}
