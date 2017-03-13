#include "boneMesh.h"
#include "openGLFunctions.h"
#include "logger.h"
#include "boneModel.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

BoneMesh::BoneMesh(){}
BoneMesh::BoneMesh(aiMesh* assimpMesh, std::vector<aiNode*> nodes, std::vector<aiNodeAnim*> animNodes)
{
    load(assimpMesh, nodes, animNodes);
}
BoneMesh::~BoneMesh()
{
    for(auto bone : bones)
        delete bone;
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

void BoneMesh::load(aiMesh* assimpMesh, std::vector<aiNode*> nodes, std::vector<aiNodeAnim*> animNodes)
{
    assimpNodes = nodes;
    assimpAnimNodes = animNodes;
    sceneInverseBaseTransform = glm::inverse(AToGMat(assimpNodes[0]->mTransformation));

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

        Bone* bone = new Bone();
        bone->id = i;
        bone->name = assimpBone->mName.C_Str();
        bone->offsetMatrix = AToGMat(assimpBone->mOffsetMatrix);
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

    //Initial transform and parenting
    for(unsigned int i = 0; i < bones.size(); i++)
    {
        aiNode* node = FindNode(bones[i]->name);
        aiNodeAnim* animNode = FindAnimNode(bones[i]->name);
        if(animNode == nullptr)
        {
            bones[i]->hasAnim = false;
        }
        else
        {
            bones[i]->hasAnim = true;
            bones[i]->animNode = AnimationNode(animNode);
        }
        Bone * parentBone = FindBone(node->mParent->mName.C_Str());
        if(!parentBone)
        {
            Logger(1) << "No parent " << node->mParent->mName.C_Str() << " found for " << bones[i]->name;
        }
        else
        {
            bones[i]->parentBone = parentBone;
        }
        glm::mat4 transform = AToGMat(node->mTransformation);

        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 position;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);
        bones[i]->transform = transform;
        bones[i]->position = position;
        bones[i]->scale = scale;
        bones[i]->rotation = rotation;
    }

    transformBones(0);
    genBuffers();
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

aiNodeAnim* BoneMesh::FindAnimNode(std::string findThis)
{
    for(unsigned int i = 0; i < assimpAnimNodes.size(); i++)
    {
        if(assimpAnimNodes[i]->mNodeName.C_Str() == findThis)
            return assimpAnimNodes[i];
    }
    return nullptr;
}

Bone* BoneMesh::FindBone(std::string findThis)
{
    for(unsigned int i = 0; i < bones.size(); i++)
    {
        if(bones[i]->name == findThis)
            return bones[i];
    }
    return nullptr;
}

void BoneMesh::transformBones(float time)
{
    boneMats.clear();
    for(unsigned int i = 0; i < bones.size(); i++)
    {
        Bone* bone = bones[i];

        glm::mat4 transform;
        if(bone->hasAnim)
        {
            bone->position = bone->InterpolatePosition(time);
            bone->rotation = bone->InterpolateRotation(time);
            bone->scale = bone->InterpolateScaling(time);
        }
        transform *= glm::translate(bone->position);
        transform *= glm::scale(bone->scale);
        transform *= glm::mat4_cast(bone->rotation);
        bone->transform = transform;
    }

    for(unsigned int i = 0; i < bones.size(); i++)
    {
        Bone* bone = bones[i];
        Bone* parent = bone->parentBone;
        std::vector<glm::mat4> mats;
        while(parent != nullptr)
        {
            mats.push_back(parent->transform);

            parent = parent->parentBone;
        }
        glm::mat4 m;
        m *= sceneInverseBaseTransform;
        for(int j = mats.size()-1; j >= 0; j--)
        {
            m *= mats[j];
        }
        m *= bone->transform;
        m *= bone->offsetMatrix;

        boneMats.push_back(m);
    }
}

unsigned int Bone::PositionIndex(float time)
{
    for(unsigned int i = 0; i < animNode.mNumPositionKeys-1; i++)
    {
        if(time > animNode.mPositionKeys[i].mTime)
            if(time < animNode.mPositionKeys[i+1].mTime)
                return i;
    }
    return 0;
}
glm::vec3 Bone::InterpolatePosition(float time)
{
    if(animNode.mNumPositionKeys < 1)
        return glm::vec3(0,0,0);
    if(animNode.mNumPositionKeys == 1)
        return animNode.mPositionKeys[0].mValue;

    unsigned int nodeIndex = PositionIndex(time);
    unsigned int nextNodeIndex = nodeIndex+1;

    glm::vec3 nodePosition = animNode.mPositionKeys[nodeIndex].mValue;
    float nodeTime = animNode.mPositionKeys[nodeIndex].mTime;
    glm::vec3 nextNodePosition = animNode.mPositionKeys[nextNodeIndex].mValue;
    float nextNodeTime = animNode.mPositionKeys[nextNodeIndex].mTime;

    float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

    return glm::mix(nodePosition, nextNodePosition, between);
}
unsigned int Bone::RotationIndex(float time)
{
    for(unsigned int i = 0; i < animNode.mNumRotationKeys-1; i++)
    {
        if(time > animNode.mRotationKeys[i].mTime)
            if(time < animNode.mRotationKeys[i+1].mTime)
                return i;
    }
    return 0;
}
glm::quat Bone::InterpolateRotation(float time)
{
    if(animNode.mNumRotationKeys < 1)
        return glm::quat(1,0,0,0);
    if(animNode.mNumRotationKeys == 1)
        return animNode.mRotationKeys[0].mValue;

    unsigned int nodeIndex = RotationIndex(time);
    unsigned int nextNodeIndex = nodeIndex+1;

    glm::quat nodeRotation = animNode.mRotationKeys[nodeIndex].mValue;
    float nodeTime = animNode.mRotationKeys[nodeIndex].mTime;
    glm::quat nextNodeRotation = animNode.mRotationKeys[nextNodeIndex].mValue;
    float nextNodeTime = animNode.mRotationKeys[nextNodeIndex].mTime;

    float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

    return glm::mix(nodeRotation, nextNodeRotation, between);
}
unsigned int Bone::ScalingIndex(float time)
{
    for(unsigned int i = 0; i < animNode.mNumScalingKeys-1; i++)
    {
        if(time > animNode.mScalingKeys[i].mTime)
            if(time < animNode.mScalingKeys[i+1].mTime)
               return i;
    }
    return 0;
}
glm::vec3 Bone::InterpolateScaling(float time)
{
    if(animNode.mNumScalingKeys < 1)
        return glm::vec3(1,1,1);
    if(animNode.mNumScalingKeys == 1)
        return animNode.mScalingKeys[0].mValue;

    unsigned int nodeIndex = ScalingIndex(time);
    unsigned int nextNodeIndex = nodeIndex+1;

    glm::vec3 nodeScaling = animNode.mScalingKeys[nodeIndex].mValue;
    float nodeTime = animNode.mScalingKeys[nodeIndex].mTime;
    glm::vec3 nextNodeScaling = animNode.mScalingKeys[nextNodeIndex].mValue;
    float nextNodeTime = animNode.mScalingKeys[nextNodeIndex].mTime;

    float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

    return glm::mix(nodeScaling, nextNodeScaling, between);
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

AnimationNode::AnimationNode()
{
    mName = nullptr;
}
AnimationNode::AnimationNode(aiNodeAnim* node)
{
    if(node == nullptr)
    {
        mName = nullptr;
        return;
    }

    mName = node->mNodeName.C_Str();
    mNumPositionKeys = node->mNumPositionKeys;
    mNumRotationKeys = node->mNumRotationKeys;
    mNumScalingKeys = node->mNumScalingKeys;

    for(int i = 0; i < mNumPositionKeys; i++)
    {
        aiVector3D p = node->mPositionKeys[i].mValue;
        VectorKey key;
        key.mValue = glm::vec3(p.x,p.y,p.z);
        key.mTime = node->mPositionKeys[i].mTime;
        mPositionKeys.push_back(key);
    }

    for(int i = 0; i < mNumRotationKeys; i++)
    {
        aiQuaternion p = node->mRotationKeys[i].mValue;
        QuatKey key;
        key.mValue = glm::quat(p.w,p.x,p.y,p.z);
        key.mTime = node->mRotationKeys[i].mTime;
        mRotationKeys.push_back(key);
    }

    for(int i = 0; i < mNumScalingKeys; i++)
    {
        aiVector3D p = node->mScalingKeys[i].mValue;
        VectorKey key;
        key.mValue = glm::vec3(p.x,p.y,p.z);
        key.mTime = node->mScalingKeys[i].mTime;
        mScalingKeys.push_back(key);
    }
}
