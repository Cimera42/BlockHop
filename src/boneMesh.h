#ifndef BONEMESH_H_INCLUDED
#define BONEMESH_H_INCLUDED

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include "mesh.h"

struct BoneVertex
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;
    int materialIndex;
    int BoneIds[4] = {0,0,0,0};
    float BoneWeights[4] = {0,0,0,0};
};

struct VectorKey
{
    glm::vec3 mValue;
    float mTime;
};

struct QuatKey
{
    glm::quat mValue;
    float mTime;
};

struct AnimationNode
{
    AnimationNode();
    AnimationNode(aiNodeAnim* node);

    const char* mName;
    unsigned int mNumPositionKeys;
    unsigned int mNumRotationKeys;
    unsigned int mNumScalingKeys;

    std::vector<VectorKey> mPositionKeys;
    std::vector<QuatKey> mRotationKeys;
    std::vector<VectorKey> mScalingKeys;
};

struct Bone
{
    int id;
    std::string name;
    glm::mat4 offsetMatrix;
    glm::mat4 transform;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    glm::mat4 finalTransform;

    Bone* parentBone = nullptr;

    bool hasAnim;
    AnimationNode animNode;
    unsigned int PositionIndex(float time);
    glm::vec3 InterpolatePosition(float time);
    unsigned int RotationIndex(float time);
    glm::quat InterpolateRotation(float time);
    unsigned int ScalingIndex(float time);
    glm::vec3 InterpolateScaling(float time);
};

class BoneMesh : public Mesh
{
    public:
        BoneMesh();
        BoneMesh(aiMesh* assimpMesh, std::vector<aiNode*> nodes, std::vector<aiNodeAnim*> animNodes);
        ~BoneMesh();

        std::vector<aiNode*> assimpNodes;
        std::vector<aiNodeAnim*> assimpAnimNodes;

        std::vector<Bone*> bones;

        glm::mat4 sceneInverseBaseTransform;
        std::vector<glm::mat4> boneMats;
        std::vector<BoneVertex> collatedVertices;

        void createVAO();
        void genBuffers();
        void load(aiMesh* assimpMesh, std::vector<aiNode*> nodes, std::vector<aiNodeAnim*> animNodes);
        void loadWithVectors(std::vector<glm::vec3> inVertices,
                             std::vector<glm::vec2> inUvs,
                             std::vector<glm::vec3> inNormals,
                             std::vector<unsigned int> inIndices);

        aiNode* FindNode(std::string findThis);
        aiNodeAnim* FindAnimNode(std::string findThis);
        Bone* FindBone(std::string findThis);
        void transformBones(float time);
};

#endif // BONEMESH_H_INCLUDED
