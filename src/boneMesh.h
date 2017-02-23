#ifndef BONEMESH_H_INCLUDED
#define BONEMESH_H_INCLUDED

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
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

struct Bone
{
    int id;
    std::string name;
    glm::mat4 offsetMatrix;
    glm::mat4 transform;
    glm::mat4 finalTransform;

    Bone* parentBone = nullptr;
};

class BoneMesh : public Mesh
{
    public:
        BoneMesh();
        BoneMesh(aiMesh* assimpMesh, std::vector<aiNode*> nodes);

        std::vector<aiNode*> assimpNodes;

        std::vector<Bone> bones;

        std::vector<glm::mat4> boneMats;
        std::vector<BoneVertex> collatedVertices;

        void createVAO();
        void genBuffers();
        void load(aiMesh* assimpMesh, std::vector<aiNode*> nodes);
        void loadWithVectors(std::vector<glm::vec3> inVertices,
                             std::vector<glm::vec2> inUvs,
                             std::vector<glm::vec3> inNormals,
                             std::vector<unsigned int> inIndices);

        aiNode* FindNode(std::string findThis);
        Bone* FindBone(std::string findThis);
        void transformBones();
};

#endif // BONEMESH_H_INCLUDED
