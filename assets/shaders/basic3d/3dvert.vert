#version 420

layout(location=0) in vec3 vertPos;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNorm;
layout(location=3) in int vertMaterialIndex;

out vec3 vPos;
out vec2 vUV;
out vec3 vNorm;
flat out int vMaterialIndex;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    vPos = vec3(modelMat * vec4(vertPos,1));
    vUV = vertUV;
    vMaterialIndex = vertMaterialIndex;

    mat3 normalMatrix = mat3(modelMat);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    vNorm = normalize(normalMatrix * vertNorm);
    vec3 a = vec3(0);
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos+a,1);
}
