#ifndef BONEMODEL_H_INCLUDED
#define BONEMODEL_H_INCLUDED

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <string>

#include "texture.h"
#include "boneMesh.h"
#include "camera.h"

struct Part
{
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    int mesh;

    std::string name;
};

struct Material
{
    std::string texturePath;
};

class BoneModel
{
    public:
        BoneModel(std::string inFilename);
        ~BoneModel();

        std::string filename;
        std::vector<Material> materials;
        Texture texture;

        std::map<unsigned int, Mesh*> normalMeshes;
        std::map<unsigned int, BoneMesh*> boneMeshes;
        std::vector<Part> parts;

        std::vector<aiNode*> assimpNodes;
        std::vector<aiNodeAnim*> animNodes;

        void load();
        void nodeLoop(aiNode* assimpNode, int indent, glm::mat4 incrementalTransform);

        void renderModel(Shader plainShader, Shader boneShader, Camera camera);
};

glm::mat4 AToGMat(aiMatrix4x4 aiMat);

#endif // BONEMODEL_H_INCLUDED
