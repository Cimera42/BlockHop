#version 420

layout(location=0) in vec3 vertPos;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNorm;
layout(location=3) in int vertMaterialIndex;
layout(location=4) in ivec4 boneIds;
layout(location=5) in vec4 boneWeights;

layout(location=0) out vec3 outPos;
layout(location=1) out vec2 outUV;
layout(location=2) out vec3 outNorm;
layout(location=3) flat out int outMaterialIndex;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 boneMats[200];

void main()
{
    mat4 BoneTransform = mat4(1.0);
	BoneTransform  = boneMats[boneIds[0]] * boneWeights[0];
	BoneTransform += boneMats[boneIds[1]] * boneWeights[1];
	BoneTransform += boneMats[boneIds[2]] * boneWeights[2];
	BoneTransform += boneMats[boneIds[3]] * boneWeights[3];

    outPos = vec3(modelMat * BoneTransform * vec4(vertPos,1));

    gl_Position = projMat * viewMat * modelMat * BoneTransform * vec4(vertPos,1);

    mat3 normalMatrix = mat3(modelMat * BoneTransform);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    outNorm = normalize(normalMatrix * vertNorm);

    outUV = vertUV;
    outMaterialIndex = vertMaterialIndex;
}
