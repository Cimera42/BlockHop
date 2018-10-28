//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_BONEMODELCOMPONENT_H
#define BLOCKHOP_BONEMODELCOMPONENT_H

#include "../ecs/component.h"
#include "../loaders/imageAsset.h"
#include "../mesh.h"
#include "../boneMesh.h"
#include "../loaders/modelAsset.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>

glm::mat4 AToGMat(aiMatrix4x4 aiMat);

struct NodePart;
struct NodeChanging
{
	NodeChanging(NodePart* node) {
		NodeChanging::node = node;
		NodeChanging::localMatrix = glm::mat4();
		NodeChanging::collectiveMatrix = glm::mat4();
		NodeChanging::nodeChParent = nullptr;
	}

	NodePart* node;
	glm::mat4 localMatrix;
	glm::mat4 collectiveMatrix;

	NodeChanging* nodeChParent;
	std::vector<NodeChanging*> nodeChChildren;
};
struct Bone;
struct BoneMesh;
struct BoneChanging
{
	BoneChanging(Bone* bone, NodeChanging* node) {
		BoneChanging::bone = bone;
		BoneChanging::node = node;
	}

	Bone* bone;
	NodeChanging* node;
};
struct BoneMeshChanging
{
	BoneMeshChanging(BoneMesh* boneMesh, NodeChanging* node) {
		BoneMeshChanging::boneMesh = boneMesh;
		BoneMeshChanging::node = node;
	}

	NodeChanging* node;

	BoneMesh* boneMesh;
	std::vector<BoneChanging*> changingBones;
	std::vector<glm::mat4> boneMats;

	void transformBones(glm::mat4 inverseMesh);

	~BoneMeshChanging();
};

class ModelAsset;
class AnimatedModelComponent : public Component<AnimatedModelComponent>
{
public:
	AnimatedModelComponent();
	~AnimatedModelComponent();
	void setValues(json inValues);

	bool animated = false;
	ModelAsset* modelAsset;
	std::map<std::string, NodeChanging*> changingNodes;
	std::map<std::string, BoneMeshChanging*> changingBoneMeshes;

	float time = 0;
	Animation* currentAnimation;

	void load();

	friend std::ostream &operator<< (std::ostream &os, AnimatedModelComponent const &c) {
		os << "AnimatedModelComponent: ";
		return os;
	}

	void transformNodes(float dt);
	void recursiveTransform(NodeChanging *node);
	NodeChanging* FindChangingNode(std::string findThis);
	BoneMeshChanging* FindChangingBoneMesh(std::string findThis);
	bool playAnimation(std::string name);

	void nodeFamilySetup();
};

#endif //BLOCKHOP_BONEMODELCOMPONENT_H
