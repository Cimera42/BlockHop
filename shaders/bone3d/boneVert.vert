#version 420

layout(location=0) in vec3 vertPos;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNorm;
layout(location=3) in int vertMaterialIndex;
layout(location=4) in ivec4 boneIds;
layout(location=5) in vec4 boneWeights;

out vec3 vPos;
out vec2 vUV;
out vec3 vNorm;
flat out int vMaterialIndex;
out vec3 vColour;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 boneMats[200];

void main()
{
    mat4 BoneTransform = mat4(1.0);
    //if(gl_VertexID == 2)
    {
        //if(boneIds[0] == 0)
        {
            //if(boneIds[1] == 1)
            {
            if(boneIds[0] == 0)
                BoneTransform = boneMats[boneIds[0]] * boneWeights[0];
                //BoneTransform += boneMats[boneIds[1]] * boneWeights[1];
            }
        }
        //BoneTransform += boneMats[boneIds[1]] * boneWeights[1];
        //BoneTransform += boneMats[boneIds[2]] * boneWeights[2];
        //BoneTransform += boneMats[boneIds[3]] * boneWeights[3];
    }
    
    float weight = 0;
    for(int i = 0; i < 4; i++)
    {
        if(boneIds[i] == 0)
        {
            weight = boneWeights[i];
            break;
        }
    }
    
    vColour = mix(vec3(0,0,1), vec3(0,1,0), smoothstep(0.0,0.5,weight));
    vColour = mix(vColour, vec3(1,0,0), smoothstep(0.5,1.0,weight));

    vPos = vec3(modelMat * BoneTransform * vec4(vertPos,1));

    gl_Position = projMat * viewMat * modelMat * BoneTransform * vec4(vertPos,1);

    mat3 normalMatrix = mat3(modelMat * BoneTransform);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    vNorm = normalize(normalMatrix * vertNorm);

    vUV = vertUV;
    vMaterialIndex = vertMaterialIndex;
}
