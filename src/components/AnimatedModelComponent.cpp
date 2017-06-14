//
// Created by Tim on 25/05/2017.
//

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "AnimatedModelComponent.h"
#include "../ecs/ecsManager.h"
#include "../assetClasses/modelAsset.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

COMPONENT_EXPORT(AnimatedModelComponent, "animatedModelComponent")

glm::mat4 AToGMat(aiMatrix4x4 aiMat)
{
	glm::mat4 glmMatrix(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
						aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
						aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
						aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
	return glmMatrix;
}

AnimatedModelComponent::AnimatedModelComponent() {}
AnimatedModelComponent::~AnimatedModelComponent() 
{
}

void AnimatedModelComponent::setValues(json inValues) 
{
    //Will throw if incorrect/should automatically be caught by ECSManager
	modelAsset = new ModelAsset(inValues["filename"].get<std::string>());
	load();
}

void AnimatedModelComponent::load()
{
	modelAsset->load();
	currentAnimation = modelAsset->animations.begin()->first;
	for(std::pair<const std::string, NodePart*> pair : modelAsset->nodeParts)
	{
		NodePart* node = pair.second;
		changingNodes[node->name] = new NodeChanging();
		changingNodes[node->name]->node = node;
	}
	for(std::pair<const unsigned int, BoneMesh*> pair : modelAsset->boneMeshes)
	{
		BoneMesh* boneMesh = pair.second;
		changingBoneMeshes[boneMesh->name] = new BoneMeshChanging();
		changingBoneMeshes[boneMesh->name]->boneMesh = boneMesh;
	}	
	nodeFamilySetup();
	transformNodes(0);
}

void AnimatedModelComponent::nodeFamilySetup()
{
	for(std::pair<const std::string, NodeChanging*> pair : changingNodes)
	{
		NodeChanging *chNode = pair.second;

		if(chNode->node->nodeParent != nullptr)
			chNode->nodeChParent = FindChangingNode(chNode->node->nodeParent->name);
		for(NodePart *node : chNode->node->nodeChildren)
		{
			chNode->nodeChChildren.push_back(FindChangingNode(node->name));
		}
	}
}

void AnimatedModelComponent::transformNodes(float dt)
{
	if(time > modelAsset->animations[currentAnimation]->duration)
		time = 0;
	time += dt*modelAsset->animations[currentAnimation]->tickRate;
	for(std::pair<const std::string, NodeChanging*> pair : changingNodes)
	{
		NodeChanging* chNode = pair.second;
		NodePart* node = chNode->node;

		glm::mat4 transform;
		glm::vec3 position = node->InterpolatePosition(time, modelAsset->animations[currentAnimation]);
		glm::quat rotation = node->InterpolateRotation(time, modelAsset->animations[currentAnimation]);
		glm::vec3 scale = node->InterpolateScaling(time, modelAsset->animations[currentAnimation]);
		transform *= glm::translate(position);
		transform *= glm::mat4_cast(rotation);
		transform *= glm::scale(scale);
		if(node->name == "Armature")
			chNode->localMatrix = glm::mat4();
		else
			chNode->localMatrix = transform;
	}
	recursiveTransform(changingNodes["RootNode"]);

	for(std::pair<const std::string, BoneMeshChanging*> pair : changingBoneMeshes)
	{
		BoneMeshChanging* chBone = pair.second;
		chBone->transformBones(changingNodes);
	}
}

NodeChanging* AnimatedModelComponent::FindChangingNode(std::string findThis)
{
	std::map<std::string, NodeChanging*>::iterator t = changingNodes.find(findThis);
	if(t != changingNodes.end())
		return t->second;
	return nullptr;
}

BoneMeshChanging* AnimatedModelComponent::FindChangingBoneMesh(std::string findThis)
{
	std::map<std::string, BoneMeshChanging*>::iterator t = changingBoneMeshes.find(findThis);
	if(t != changingBoneMeshes.end())
		return t->second;
	return nullptr;
}

void AnimatedModelComponent::recursiveTransform(NodeChanging *chNode)
{
	if(chNode->nodeChParent)
	{
		chNode->collectiveMatrix = chNode->nodeChParent->collectiveMatrix;
	}
	chNode->collectiveMatrix *= chNode->localMatrix;

	for(auto nodeChildren : chNode->nodeChChildren)
		recursiveTransform(nodeChildren);
}

bool AnimatedModelComponent::playAnimation(std::string name)
{
	Animation* anim = modelAsset->FindAnim(name);
	if(anim == nullptr)
		return false;
	
	time = 0;
	currentAnimation = name;
	return true;
}

void BoneMeshChanging::transformBones(std::map<std::string, NodeChanging*> chNodes)
{
	boneMats.clear();
	//Update matrices of all bones
	for(unsigned int i = 0; i < boneMesh->bones.size(); i++)
	{
		Bone* bone = boneMesh->bones[i];

		NodeChanging* chNode = FindChangingNode(chNodes, bone->name);
		boneMats.push_back(chNode->collectiveMatrix * bone->offsetMatrix);

		if(bone->name == "Bone.005")
		{
			glm::vec3 scaleb;
			glm::quat rotationb;
			glm::vec3 positionb;
			glm::vec3 skewb;
			glm::vec4 perspectiveb;
			glm::decompose(chNode->collectiveMatrix * bone->offsetMatrix, scaleb, rotationb, positionb, skewb, perspectiveb);
			Logger(1) << "Bone: " << bone->name << " - Mesh: " << this->boneMesh->name;
			Logger(1) << "    Position: " << positionb;
			Logger(1) << "    Rotation: " << rotationb;
			Logger(1) << "    Scale: " << scaleb;
		}
	}
}

NodeChanging *BoneMeshChanging::FindChangingNode(std::map<std::string, NodeChanging*> chNodes, std::string findThis)
{
	std::map<std::string, NodeChanging*>::iterator t = chNodes.find(findThis);
	if(t != chNodes.end())
		return t->second;
	return nullptr;
}
