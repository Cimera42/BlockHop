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
	AnimationNode(aiNodeAnim* animNode, aiNode* node);

	const char* mName;
	unsigned int mNumPositionKeys;
	unsigned int mNumRotationKeys;
	unsigned int mNumScalingKeys;

	std::vector<VectorKey> mPositionKeys;
	std::vector<QuatKey> mRotationKeys;
	std::vector<VectorKey> mScalingKeys;
};

struct NodePart
{
	glm::mat4 defaultTransform;
	glm::mat4 localMatrix;
	glm::mat4 collectiveMatrix;
	AnimationNode* animationNode;
	
	unsigned int PositionIndex(float time);
	glm::vec3 InterpolatePosition(float time);

	unsigned int RotationIndex(float time);
	glm::quat InterpolateRotation(float time);

	unsigned int ScalingIndex(float time);
	glm::vec3 InterpolateScaling(float time);
	
	std::string name;
	NodePart* nodeParent;
	std::vector<NodePart*> nodeChildren;
};

struct MeshPart
{
	int mesh;

	NodePart* nodeParent;
};

struct Material
{
    std::string texturePath;
};

class BoneMesh;
class AnimatedModelComponent : public Component
{
    static bool exported;
public:
    AnimatedModelComponent();
    ~AnimatedModelComponent();
    void setValues(json inValues);

	std::string filename;
	std::vector<Material> materials;
	Texture texture;

	std::map<unsigned int, Mesh*> normalMeshes;
	std::map<unsigned int, BoneMesh*> boneMeshes;
	std::vector<MeshPart*> meshParts;
	std::vector<NodePart*> nodeParts;

	std::vector<aiNode*> assimpNodes;
	std::vector<aiNodeAnim*> animNodes;

	void load();
	NodePart* nodeLoop(aiNode *assimpNode, int indent, NodePart *parent);
	aiNodeAnim* FindAnimNode(std::string findThis);
	
    friend std::ostream &operator<< (std::ostream &os, AnimatedModelComponent const &c) {
        os << "AnimatedModelComponent: ";
        os << c.filename;
        return os;
    }

	void transformNodes();
	void recursiveTransform(NodePart *node);
};

#endif //BLOCKHOP_BONEMODELCOMPONENT_H
