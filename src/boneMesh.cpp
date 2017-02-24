#include "boneMesh.h"
#include "openGLFunctions.h"
#include "logger.h"
#include "boneModel.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

BoneMesh::BoneMesh(){}
BoneMesh::BoneMesh(aiMesh* assimpMesh, std::vector<aiNode*> nodes)
{
    load(assimpMesh, nodes);
}

void BoneMesh::createVAO()
{
    glGenVertexArrays(1, &VAO);
    glSetBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        //Enable vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        int inc = 0;
        GLuint vertLoc = 0;
        glEnableVertexAttribArray(vertLoc);
        glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*) (intptr_t) inc); //attribute, size, type, is normalised?, stride, offset
        inc += sizeof(glm::vec3);

        GLuint uvLoc = 1;
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*) (intptr_t) inc);
        inc += sizeof(glm::vec2);

        GLuint normLoc = 2;
        glEnableVertexAttribArray(normLoc);
        glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*) (intptr_t) inc);
        inc += sizeof(glm::vec3);

        GLuint matLoc = 3;
        glEnableVertexAttribArray(matLoc);
        glVertexAttribIPointer(matLoc, 1, GL_INT, sizeof(BoneVertex), (void*) (intptr_t) inc);
        inc += sizeof(int);

        GLuint boneIdLoc = 4;
        glEnableVertexAttribArray(boneIdLoc);
        glVertexAttribIPointer(boneIdLoc, 4, GL_INT, sizeof(BoneVertex), (void*) (intptr_t) inc);
        inc += sizeof(int[4]);

        GLuint boneWeightLoc = 5;
        glEnableVertexAttribArray(boneWeightLoc);
        glVertexAttribPointer(boneWeightLoc, 4, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*) (intptr_t) inc);
        inc += sizeof(int[4]);
    glSetBindVertexArray(0);
}

void BoneMesh::genBuffers()
{
    //Generate VUNMBW (vertexUvNormalMaterialBoneIDBoneWeight) buffer using data
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, collatedVertices.size() * sizeof(BoneVertex), collatedVertices.data(), GL_STATIC_DRAW);

    //Generate index buffer using data
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    createVAO();
}

void BoneMesh::load(aiMesh* assimpMesh, std::vector<aiNode*> nodes)
{
    assimpNodes = nodes;

    for(unsigned int j = 0; j < assimpMesh->mNumFaces; j++)
    {
        aiFace& assimpFace = assimpMesh->mFaces[j];

        for(unsigned int k = 0; k < assimpFace.mNumIndices; k++)
        {
            indices.push_back(assimpFace.mIndices[k]);
        }
    }
    for(unsigned int j = 0; j < assimpMesh->mNumVertices; j++)
    {
        aiVector3D vertex = assimpMesh->mVertices[j];
        glm::vec3 glmVert = glm::vec3(vertex.x,vertex.y,vertex.z);
        vertices.push_back(glmVert);

        aiVector3D uv = assimpMesh->mTextureCoords[0][j];
        glm::vec2 glmUv = glm::vec2(uv.x,uv.y);
        uvs.push_back(glmUv);

        aiVector3D normal = assimpMesh->mNormals[j];
        glm::vec3 glmNormal = glm::vec3(normal.x,normal.y,normal.z);
        normals.push_back(glmNormal);

        materialIndices.push_back(assimpMesh->mMaterialIndex);

        BoneVertex collatedVertex;
        collatedVertex.pos = glmVert;
        collatedVertex.uv = glmUv;
        collatedVertex.normal = glmNormal;
        collatedVertex.materialIndex = assimpMesh->mMaterialIndex;
        collatedVertices.push_back(collatedVertex);
    }

    for(unsigned int i = 0; i < assimpMesh->mNumBones; i++)
    {
        aiBone* assimpBone = assimpMesh->mBones[i];

        Bone bone;
        bone.id = i;
        bone.name = assimpBone->mName.C_Str();
        bone.offsetMatrix = AToGMat(assimpBone->mOffsetMatrix);
        bones.push_back(bone);

        for(unsigned int j = 0; j < assimpBone->mNumWeights; j++)
        {
            aiVertexWeight assimpWeight = assimpBone->mWeights[j];

            BoneVertex vert = collatedVertices[assimpWeight.mVertexId];
            for(unsigned int k = 0; k < 4; k++)
            {
                if(vert.BoneWeights[k] <= 0.0f)
                {
                    vert.BoneIds[k] = i;
                    vert.BoneWeights[k] = assimpWeight.mWeight;
                    break;
                }
            }
            collatedVertices[assimpWeight.mVertexId] = vert;
        }
    }

    transformBones();
}

aiNode* BoneMesh::FindNode(std::string findThis)
{
    for(unsigned int i = 0; i < assimpNodes.size(); i++)
    {
        if(assimpNodes[i]->mName.C_Str() == findThis)
            return assimpNodes[i];
    }
    return nullptr;
}

Bone* BoneMesh::FindBone(std::string findThis)
{
    for(unsigned int i = 0; i < bones.size(); i++)
    {
        if(bones[i].name == findThis)
            return &bones[i];
    }
    return nullptr;
}

void BoneMesh::transformBones()
{
    for(unsigned int i = 0; i < bones.size(); i++)
    {
        aiNode* node = FindNode(bones[i].name);
        Bone* parentBone = FindBone(node->mParent->mName.C_Str());
        bones[i].parentBone = parentBone;
        if(!parentBone)
        {
            Logger(1) << "No parent " << node->mParent->mName.C_Str() << " found for " << bones[i].name;
        }
        bones[i].transform = AToGMat(node->mTransformation);
    }

    for(unsigned int i = 0; i < bones.size(); i++)
    {
        Bone bone = bones[i];
        Bone* parent = bones[i].parentBone;
        std::vector<glm::mat4> mats;
        while(parent != nullptr)
        {
            mats.push_back(parent->transform);

            parent = parent->parentBone;
        }
        glm::mat4 m;
        m *= glm::inverse(AToGMat(assimpNodes[0]->mTransformation));
        for(int j = mats.size()-1; j >= 0; j--)
        {
            m *= mats[j];
        }
        m *= bones[i].transform;
        m *= bones[i].offsetMatrix;

        boneMats.push_back(m);
    }

    genBuffers();
}

void BoneMesh::loadWithVectors(std::vector<glm::vec3> inVertices, std::vector<glm::vec2> inUvs, std::vector<glm::vec3> inNormals, std::vector<unsigned int> inIndices)
{
    vertices.swap(inVertices);
    uvs.swap(inUvs);
    normals.swap(inNormals);
    indices.swap(inIndices);
    for(unsigned int i = 0; inIndices.size(); i++)
        materialIndices.push_back(0);

    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        BoneVertex collatedVertex;
        collatedVertex.pos = vertices[i];
        collatedVertex.uv = uvs[i];
        collatedVertex.normal = normals[i];
        collatedVertex.materialIndex = materialIndices[i];
        collatedVertices.push_back(collatedVertex);
    }

    genBuffers();
}
