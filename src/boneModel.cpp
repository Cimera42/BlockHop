#include "boneModel.h"
#include "logger.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing fla
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "openGLFunctions.h"

glm::mat4 AToGMat(aiMatrix4x4 aiMat)
{
    glm::mat4 glmMatrix(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
                        aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
                        aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
                        aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
    return glmMatrix;
}

BoneModel::BoneModel(std::string inFilename)
{
    filename = inFilename;
}
BoneModel::~BoneModel()
{
    for(auto nM : normalMeshes)
        nM.second->destruct();
    for(auto bM : boneMeshes)
        bM.second->destruct();
}

void BoneModel::load()
{
    Assimp::Importer importer;
    Logger(1) << "Loading model: " << filename;
    const aiScene* scene = importer.ReadFile(filename,
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType |
                                             aiProcess_LimitBoneWeights |
                                             aiProcess_ValidateDataStructure /* |
                                             aiProcess_PreTransformVertices*/);

    if(!scene)
    {
        Logger(1) << importer.GetErrorString();
        Logger(1) << "Could not load Mesh. Error importing";
        return;
    }

    Logger(1) << "Animations: " << scene->mNumAnimations;
    for(unsigned int i = 0; i < scene->mNumAnimations; i++)
    {
        aiAnimation* assimpAnimation = scene->mAnimations[i];
        Logger(1) << "TPS: " << assimpAnimation->mTicksPerSecond;

        Logger(1) << "Animation name: " << assimpAnimation->mName.C_Str();
        Logger(1) << "Animation channels: " << assimpAnimation->mNumChannels;
        for(unsigned int j = 0; j < assimpAnimation->mNumChannels; j++)
        {
            animNodes.push_back(assimpAnimation->mChannels[j]);
        }
    }

    for(unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* assimpMaterial = scene->mMaterials[i];

        aiString nnn;
        assimpMaterial->Get(AI_MATKEY_NAME, nnn);
        if(strcmp(nnn.C_Str(), AI_DEFAULT_MATERIAL_NAME) == 0)
            continue;

        Material material;

        aiString texPath;
        if(assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
        {
            material.texturePath = std::string(texPath.C_Str());
        }

        materials.push_back(material);
    }

    std::vector<std::string> texPaths;
    for(unsigned int i = 0; i < materials.size(); i++)
    {
        if(materials[i].texturePath.find_first_not_of(' ') != std::string::npos)
        {
            std::string backslashFixed = materials[i].texturePath;
            std::replace(backslashFixed.begin(), backslashFixed.end(), '\\', '/');

            std::string baseFolder = std::string(filename);
            baseFolder = baseFolder.substr(0, baseFolder.find_last_of("/"));

            texPaths.push_back((baseFolder +"/"+ backslashFixed).c_str());
        }
    }
    if(texPaths.size() > 0)
    {
        texture.load(texPaths);
    }

    nodeLoop(scene->mRootNode, 0, glm::mat4());

    for(unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* assimpMesh = scene->mMeshes[i];
        if(assimpMesh->mNumBones > 0)
        {
            boneMeshes[i] = new BoneMesh(assimpMesh, assimpNodes, animNodes);
        }
        else
        {
            normalMeshes[i] = new Mesh(assimpMesh);
        }
    }
}

void BoneModel::nodeLoop(aiNode* assimpNode, int indent, glm::mat4 incrementalTransform)
{
    assimpNodes.push_back(assimpNode);

    aiVector3D lscale;
    aiVector3D lposition;
    aiQuaternion lrotation;
    assimpNode->mTransformation.Decompose(lscale, lrotation, lposition);

    glm::mat4 nodeMatrix = AToGMat(assimpNode->mTransformation);
    glm::mat4 newTransform = incrementalTransform * nodeMatrix;

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(newTransform, scale, rotation, position, skew, perspective);

    glm::vec3 nscale;
    glm::quat nrotation;
    glm::vec3 nposition;
    glm::vec3 nskew;
    glm::vec4 nperspective;
    glm::decompose(nodeMatrix, nscale, nrotation, nposition, nskew, nperspective);

    /*std::string indents;
    for(int i = 0; i < indent; i++)
        indents += "        ";
    Logger(1) << indents << assimpNode->mName.C_Str();

    Logger(1) << indents << "    S1: " << scale.x << " = " << scale.y << " = " << scale.z;
    Logger(1) << indents << "    P1: " << position.x << " = " << position.y << " = " << position.z;
    Logger(1) << indents << "    R1: " << rotation.x << " = " << rotation.y << " = " << rotation.z << " = " << rotation.w;

    Logger(1) << indents << "    S2: " << nscale.x << " = " << nscale.y << " = " << nscale.z;
    Logger(1) << indents << "    P2: " << nposition.x << " = " << nposition.y << " = " << nposition.z;
    Logger(1) << indents << "    R2: " << nrotation.x << " = " << nrotation.y << " = " << nrotation.z << " = " << nrotation.w;*/

    if(assimpNode->mNumMeshes)
    {
        Part part;
        part.mesh = assimpNode->mMeshes[0];
        part.position = glm::vec3(position.x, position.y, position.z);
        //Logger(1) << "Part pos: " << position.x << " = " << position.y << " = " << position.z;
        part.scale = glm::vec3(scale.x, scale.y, scale.z);
        part.rotation = rotation;
        part.name = assimpNode->mName.C_Str();
        //Logger(1) << "Part: " << part.name;
        parts.push_back(part);
    }

    for(unsigned int i = 0; i < assimpNode->mNumChildren; i++)
    {
        aiNode* childNode = assimpNode->mChildren[i];
        nodeLoop(childNode, indent+1, newTransform);
    }
}

void BoneModel::renderModel(Shader* plainShader, Shader* boneShader, Camera camera)
{
    for(unsigned int i = 0; i < parts.size(); i++)
    {
        Part part = parts[i];
        glm::mat4 modelMatrix = glm::translate(part.position);
        modelMatrix = glm::scale(modelMatrix, part.scale);
        modelMatrix = modelMatrix * glm::toMat4(part.rotation);

        if(normalMeshes.find(part.mesh) != normalMeshes.end())
        {
            Mesh* mesh = normalMeshes[part.mesh];
            plainShader->use();
            glUniformMatrix4fv(plainShader->getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
            glUniformMatrix4fv(plainShader->getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

            glSetActiveTexture(GL_TEXTURE0);
            glSetBindTexture(GL_TEXTURE_2D_ARRAY, texture.textureID);
            glUniform1i(plainShader->getLoc("textureSampler"), 0);

            glUniformMatrix4fv(plainShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

            glSetBindVertexArray(mesh->VAO);
            glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
            glSetBindVertexArray(0);
        }
        else
        {
            BoneMesh* boneMesh = boneMeshes[part.mesh];
            boneShader->use();
            glUniformMatrix4fv(boneShader->getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
            glUniformMatrix4fv(boneShader->getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

            glSetActiveTexture(GL_TEXTURE0);
            glSetBindTexture(GL_TEXTURE_2D_ARRAY, texture.textureID);
            glUniform1i(boneShader->getLoc("textureSampler"), 0);

            glUniformMatrix4fv(boneShader->getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

            unsigned int matsNum = boneMesh->boneMats.size();
            if(matsNum)
            {
                boneMesh->transformBones((float) glfwGetTime()*24.0f);
                int matsLoc = boneShader->getLoc("boneMats");
                glUniformMatrix4fv(matsLoc, matsNum,
                                   GL_FALSE, &boneMesh->boneMats.data()[0][0][0]);
            }

            glSetBindVertexArray(boneMesh->VAO);
            glDrawElements(GL_TRIANGLES, boneMesh->indices.size(), GL_UNSIGNED_INT, 0);
            glSetBindVertexArray(0);
        }
    }
}