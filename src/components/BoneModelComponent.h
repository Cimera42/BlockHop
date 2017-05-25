//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_BONEMODELCOMPONENT_H
#define BLOCKHOP_BONEMODELCOMPONENT_H

#include "../ecs/component.h"
#include "../texture.h"
#include "../mesh.h"
#include "../boneMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>

glm::mat4 AToGMat(aiMatrix4x4 aiMat);

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


class BoneModelComponent : public Component
{
    static bool exported;
public:
    BoneModelComponent();
    ~BoneModelComponent();
    void setValues(json inValues);

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
	
    friend std::ostream &operator<< (std::ostream &os, BoneModelComponent const &c) {
        os << "BoneModelComponent";
        return os;
    }
};

#endif //BLOCKHOP_BONEMODELCOMPONENT_H
