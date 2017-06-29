//
// Created by Tim on 25/05/2017.
//

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "AnimatedModelComponent.h"
#include "../ecs/ecsManager.h"
#include "../loaders/assetManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

COMPONENT_EXPORT(AnimatedModelComponent, "animatedModelComponent")

/*
 * Convert from Assimp 4x4 matrix to GLM 4x4 matrix
 */
glm::mat4 AToGMat(aiMatrix4x4 aiMat)
{
	glm::mat4 glmMatrix(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
						aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
						aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
						aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
	return glmMatrix;
}

AnimatedModelComponent::AnimatedModelComponent() {}
AnimatedModelComponent::~AnimatedModelComponent() {}

void AnimatedModelComponent::setValues(json inValues) 
{
    //Will throw if incorrect/should automatically be caught by ECSManager
	modelAsset = static_cast<ModelAsset*>(AssetManager::i()->loadSync(inValues["filename"].get<std::string>()));
	load();
}

void AnimatedModelComponent::load()
{
	//If there are available animations, animate the model
	if(modelAsset->animations.size() > 0)
	{
		animated = true;
		//Get a default animation to use
		currentAnimation = modelAsset->animations.begin()->second;
		
		//Create a changeable node above the shared asset
		for(std::pair<const std::string, NodePart*> pair : modelAsset->nodeParts)
		{
			NodePart* node = pair.second;
			changingNodes[node->name] = new NodeChanging(node);
		}

		//Create a changeable node above the shared asset
		for(std::pair<const unsigned int, BoneMesh*> pair : modelAsset->boneMeshes)
		{
			BoneMesh* boneMesh = pair.second;
			BoneMeshChanging* boneMeshChanging = new BoneMeshChanging(boneMesh, FindChangingNode(boneMesh->name));
			//Link between bone and changing node to save on string finds
			for(auto bone : boneMesh->bones)
			{
				boneMeshChanging->changingBones.push_back(new BoneChanging(bone, FindChangingNode(bone->name)));
			}
			changingBoneMeshes[boneMesh->name] = boneMeshChanging;
		}
		nodeFamilySetup();
		transformNodes(0);
	}
}

/*
 * Link parents and children
 */
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

/*
 * Animate nodes using delta time
 */
void AnimatedModelComponent::transformNodes(float dt)
{
	if(animated)
	{
		//Repeat animation if over
		//TODO dynamic behaviour on animation end
		if (time > currentAnimation->duration)
			time = 0;
		time += dt * currentAnimation->tickRate;

		//Find local transforms of all nodes
		for (std::pair<const std::string, NodeChanging *> pair : changingNodes)
		{
			NodeChanging *chNode = pair.second;
			NodePart *node = chNode->node;

			glm::mat4 transform;
			glm::vec3 position = node->InterpolatePosition(time, currentAnimation);
			glm::quat rotation = node->InterpolateRotation(time, currentAnimation);
			glm::vec3 scale = node->InterpolateScaling(time, currentAnimation);

			transform *= glm::translate(position);
			transform *= glm::mat4_cast(rotation);
			transform *= glm::scale(scale);

			chNode->localMatrix = transform;
		}
		//Recursively collate transforms
		recursiveTransform(FindChangingNode(modelAsset->rootNodeName));

		//Animate bone meshes
		for (std::pair<const std::string, BoneMeshChanging *> pair : changingBoneMeshes)
		{
			BoneMeshChanging *chBone = pair.second;
			//Convert from world space back into bone space
			glm::mat4 inv = glm::inverse(chBone->node->collectiveMatrix);
			chBone->transformBones(inv);
		}
	}
}

NodeChanging* AnimatedModelComponent::FindChangingNode(std::string findThis)
{
	auto t = changingNodes.find(findThis);
	if(t != changingNodes.end())
		return t->second;
	return nullptr;
}

BoneMeshChanging* AnimatedModelComponent::FindChangingBoneMesh(std::string findThis)
{
	auto t = changingBoneMeshes.find(findThis);
	if(t != changingBoneMeshes.end())
		return t->second;
	return nullptr;
}

/*
 * Recursively collate transforms
 */
void AnimatedModelComponent::recursiveTransform(NodeChanging *chNode)
{
	//Root node won't have a parent
	if(chNode->nodeChParent)
	{
		chNode->collectiveMatrix = chNode->nodeChParent->collectiveMatrix;
	}
	chNode->collectiveMatrix *= chNode->localMatrix;

	//Transform children
	for(auto nodeChildren : chNode->nodeChChildren)
		recursiveTransform(nodeChildren);
}

/*
 * Find animation by name and play
 */
bool AnimatedModelComponent::playAnimation(std::string name)
{
	Animation* anim = modelAsset->FindAnim(name);
	if(anim == nullptr)
		return false;
	
	time = 0;
	currentAnimation = anim;
	return true;
}

/*
 * Collate bone transformations
 */
void BoneMeshChanging::transformBones(glm::mat4 inverseMesh)
{
	//Clear old transforms
	boneMats.clear();
	//Update matrices of all bones
	for(auto bone : changingBones)
	{
		boneMats.push_back(inverseMesh * bone->node->collectiveMatrix * bone->bone->offsetMatrix);
	}
}