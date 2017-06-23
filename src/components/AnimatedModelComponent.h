//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_BONEMODELCOMPONENT_H
#define BLOCKHOP_BONEMODELCOMPONENT_H

#include "../ecs/component.h"
#include "../loaders/imageAsset.h"
#include "../mesh.h"
#include "../boneMesh.h"
#include "../assetClasses/modelAsset.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>

glm::mat4 AToGMat(aiMatrix4x4 aiMat);

struct NodePart;
struct NodeChanging
{
	NodePart* node;
	glm::mat4 localMatrix;
	glm::mat4 collectiveMatrix;

	NodeChanging* nodeChParent;
	std::vector<NodeChanging*> nodeChChildren;
};
struct BoneMesh;
struct BoneMeshChanging
{
	BoneMesh* boneMesh;
	std::vector<glm::mat4> boneMats;

	NodeChanging* FindChangingNode(std::map<std::string, NodeChanging*> chNodes, std::string findThis);
	void transformBones(glm::mat4 inverseMesh, std::map<std::string, NodeChanging *> nodes);
};

class ModelAsset;
class AnimatedModelComponent : public Component
{
	static bool exported;
public:
	AnimatedModelComponent();
	~AnimatedModelComponent();
	void setValues(json inValues);

	bool animated = false;
	ModelAsset* modelAsset;
	std::map<std::string, NodeChanging*> changingNodes;
	std::map<std::string, BoneMeshChanging*> changingBoneMeshes;

	float time = 0;
	std::string currentAnimation;

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
