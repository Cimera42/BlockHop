//
// Created by Tim on 13/06/2017.
//

#include "modelAsset.h"
#include "../logger.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "../mesh.h"
#include "../boneMesh.h"
#include "assetManager.h"

AnimationNode::AnimationNode()
{
	mName = nullptr;
}
AnimationNode::AnimationNode(aiNodeAnim* animNode, aiNode* node)
{
	if(animNode == nullptr)
	{
		mName = node->mName.C_Str();
		mNumPositionKeys = 1;
		mNumRotationKeys = 1;
		mNumScalingKeys = 1;

		aiVector3D p;
		aiQuaternion r;
		aiVector3D s;
		node->mTransformation.Decompose(s, r, p);

		VectorKey pKey;
		pKey.mValue = glm::vec3(p.x, p.y, p.z);
		pKey.mTime = 0;
		mPositionKeys.push_back(pKey);

		QuatKey rKey;
		rKey.mValue = glm::quat(r.w, r.x, r.y, r.z);
		rKey.mTime = 0;
		mRotationKeys.push_back(rKey);

		VectorKey sKey;
		sKey.mValue = glm::vec3(s.x, s.y, s.z);
		sKey.mTime = 0;
		mScalingKeys.push_back(sKey);
		return;
	}

	mName = animNode->mNodeName.C_Str();
	mNumPositionKeys = animNode->mNumPositionKeys;
	mNumRotationKeys = animNode->mNumRotationKeys;
	mNumScalingKeys = animNode->mNumScalingKeys;

	for(int i = 0; i < mNumPositionKeys; i++)
	{
		aiVector3D p = animNode->mPositionKeys[i].mValue;
		VectorKey key;
		key.mValue = glm::vec3(p.x,p.y,p.z);
		key.mTime = (float) animNode->mPositionKeys[i].mTime;
		mPositionKeys.push_back(key);
	}

	for(int i = 0; i < mNumRotationKeys; i++)
	{
		aiQuaternion p = animNode->mRotationKeys[i].mValue;
		QuatKey key;
		key.mValue = glm::quat(p.w,p.x,p.y,p.z);
		key.mTime = (float) animNode->mRotationKeys[i].mTime;
		mRotationKeys.push_back(key);
	}

	for(int i = 0; i < mNumScalingKeys; i++)
	{
		aiVector3D p = animNode->mScalingKeys[i].mValue;
		VectorKey key;
		key.mValue = glm::vec3(p.x,p.y,p.z);
		key.mTime = (float) animNode->mScalingKeys[i].mTime;
		mScalingKeys.push_back(key);
	}
}

aiNodeAnim* Animation::FindAnimNode(std::string findThis)
{
	std::map<std::string, aiNodeAnim*>::iterator t = animNodes.find(findThis);
	if(t != animNodes.end())
		return t->second;
	return nullptr;
}

unsigned int NodePart::PositionIndex(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	for(unsigned int i = 0; i < animationNode->mNumPositionKeys-1; i++)
	{
		if(time > animationNode->mPositionKeys[i].mTime)
			if(time < animationNode->mPositionKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::vec3 NodePart::InterpolatePosition(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	if(animationNode->mNumPositionKeys < 1)
		return glm::vec3(0,0,0);
	if(animationNode->mNumPositionKeys == 1)
		return animationNode->mPositionKeys[0].mValue;

	unsigned int nodeIndex = PositionIndex(time,animation);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::vec3 nodePosition = animationNode->mPositionKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mPositionKeys[nodeIndex].mTime;
	glm::vec3 nextNodePosition = animationNode->mPositionKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mPositionKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodePosition, nextNodePosition, between);
}
unsigned int NodePart::RotationIndex(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	for(unsigned int i = 0; i < animationNode->mNumRotationKeys-1; i++)
	{
		if(time > animationNode->mRotationKeys[i].mTime)
			if(time < animationNode->mRotationKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::quat NodePart::InterpolateRotation(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	if(animationNode->mNumRotationKeys < 1)
		return glm::quat(1,0,0,0);
	if(animationNode->mNumRotationKeys == 1)
		return animationNode->mRotationKeys[0].mValue;

	unsigned int nodeIndex = RotationIndex(time,animation);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::quat nodeRotation = animationNode->mRotationKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mRotationKeys[nodeIndex].mTime;
	glm::quat nextNodeRotation = animationNode->mRotationKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mRotationKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodeRotation, nextNodeRotation, between);
}
unsigned int NodePart::ScalingIndex(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	for(unsigned int i = 0; i < animationNode->mNumScalingKeys-1; i++)
	{
		if(time > animationNode->mScalingKeys[i].mTime)
			if(time < animationNode->mScalingKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::vec3 NodePart::InterpolateScaling(float time, Animation* animation)
{
	AnimationNode* animationNode = animation->animationNodes[name];
	if(animationNode->mNumScalingKeys < 1)
		return glm::vec3(1,1,1);
	if(animationNode->mNumScalingKeys == 1)
		return animationNode->mScalingKeys[0].mValue;

	unsigned int nodeIndex = ScalingIndex(time,animation);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::vec3 nodeScaling = animationNode->mScalingKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mScalingKeys[nodeIndex].mTime;
	glm::vec3 nextNodeScaling = animationNode->mScalingKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mScalingKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodeScaling, nextNodeScaling, between);
}

ModelAsset::ModelAsset(std::string inFilename) : BaseAsset(inFilename) {}

ModelAsset::~ModelAsset()
{
	for(auto nM : normalMeshes)
		nM.second->destruct();
	for(auto bM : boneMeshes)
		bM.second->destruct();
}

void ModelAsset::load()
{
	Assimp::Importer importer;
	Logger(1) << "Loading model: " << filename;
	const aiScene* scene = importer.ReadFile(filename,
											 aiProcess_CalcTangentSpace |
											 aiProcess_Triangulate |
											 aiProcess_JoinIdenticalVertices |
											 aiProcess_SortByPType //|
											 //aiProcess_LimitBoneWeights |
											 //aiProcess_ValidateDataStructure// |
			//aiProcess_PreTransformVertices
	);

	if(!scene)
	{
		Logger(1) << importer.GetErrorString();
		Logger(1) << "Could not load Mesh. Error importing";
		return;
	}

	Logger(1) << "Animations: " << scene->mNumAnimations;
	for(unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* assimpAnimation = scene->mAnimations[i];
		Logger(1) << "Animation name: " << assimpAnimation->mName.C_Str();
		Logger(1) << "Animation channels: " << assimpAnimation->mNumChannels;
		Logger(1) << "TPS: " << assimpAnimation->mTicksPerSecond;

		Animation* anim = new Animation();
		anim->name = assimpAnimation->mName.C_Str();
		anim->channels = assimpAnimation->mNumChannels;
		anim->tickRate = assimpAnimation->mTicksPerSecond;
		anim->duration = assimpAnimation->mDuration;
		for(unsigned int j = 0; j < assimpAnimation->mNumChannels; j++)
		{
			aiNodeAnim* an = assimpAnimation->mChannels[j];
			anim->animNodes[an->mNodeName.C_Str()] = an;
		}
		animations[anim->name] = anim;
	}
	
	for(unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* assimpMaterial = scene->mMaterials[i];

		aiString nnn;
		assimpMaterial->Get(AI_MATKEY_NAME, nnn);
		if(strcmp(nnn.C_Str(), AI_DEFAULT_MATERIAL_NAME) == 0)
			continue;

		Material material = Material();

		aiString texPath;
		if(assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS)
		{
            std::string backslashFixed = texPath.C_Str();
            std::replace(backslashFixed.begin(), backslashFixed.end(), '\\', '/');

			//Convert to correct folder
			std::string baseFolder = std::string(filename);
			baseFolder = baseFolder.substr(0, baseFolder.find_last_of("/"));
            //Load image
			material.image = static_cast<ImageAsset*>(AssetManager::i()->loadSync(std::string(baseFolder +"/"+ backslashFixed)));
		}

		materials.push_back(material);
	}

    //Collate images for texture
	std::vector<std::string> texPaths;
	for(unsigned int i = 0; i < materials.size(); i++)
	{
        texPaths.push_back(materials[i].image->getName());
	}

	if(materials.size() > 0)
	{
        ImageLoader* imgLoader = static_cast<ImageLoader*>(AssetManager::i()->getLoader("image"));
        texture = imgLoader->loadTexture(texPaths);
	}

	rootNodeName = scene->mRootNode->mName.C_Str();
	nodeLoop(scene->mRootNode, 0, nullptr);

	for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* assimpMesh = scene->mMeshes[i];
		std::string meshName = assimpMesh->mName.C_Str();
		if(assimpMesh->mNumBones > 0)
		{
			boneMeshes[i] = new BoneMesh(meshName, assimpMesh, assimpNodes);
		}
		else
		{
			normalMeshes[i] = new Mesh(meshName, assimpMesh);
		}
	}
}

NodePart* ModelAsset::nodeLoop(aiNode *assimpNode, int indent, NodePart *parent)
{
	NodePart* nodePart = new NodePart();
	nodePart->name = assimpNode->mName.C_Str();
	assimpNodes[nodePart->name] = assimpNode;
	nodePart->nodeParent = parent;
	for(std::pair<const std::string, Animation*> pair : animations)
	{
		Animation* anim = pair.second;
		aiNodeAnim* a = anim->FindAnimNode(nodePart->name);
		anim->animationNodes[nodePart->name] = new AnimationNode(a, assimpNode);
	}
	//nodePart->defaultTransform = AToGMat(assimpNode->mTransformation);

//	glm::vec3 skew;
//	glm::vec4 perspective;
//	glm::decompose(nodePart->defaultTransform, scale, rotation, position, skew, perspective);

	/*
	 * Extract scale, rotation and position
	 * Then use them to cosntruct a transformation matrix
	 * This could also be done using AToGMat, which directly converts the matrix
	 * The component extraction is done anyway for logging, so why not
	 * do them both at once
	 */
	aiVector3D p;
	aiQuaternion r;
	aiVector3D s;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 position;
	assimpNode->mTransformation.Decompose(s,r,p);
	position = glm::vec3(p.x, p.y, p.z);
	rotation = glm::quat(r.w, r.x, r.y, r.z);
	scale = glm::vec3(s.x, s.y, s.z);
	nodePart->defaultTransform = glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);

	std::string indentS = [indent](){std::string c; for(int i = 0; i < indent;i++){c+="    ";} return c;}();
	Logger(1) << indentS << "Node: \"" << nodePart->name << "\"";
	Logger(1) << indentS << "    Position: " << position;
	Logger(1) << indentS << "    Rotation: " << rotation;
	Logger(1) << indentS << "    RotationA: " << glm::axis(rotation);
	Logger(1) << indentS << "    RotationE: " << glm::eulerAngles(rotation);
	Logger(1) << indentS << "    Scale: " << scale;

	nodeParts[nodePart->name] = nodePart;

	for(int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		MeshPart* meshPart = new MeshPart();
		meshPart->mesh = assimpNode->mMeshes[i];
		meshPart->nodeParent = nodePart;
		meshParts[meshPart->nodeParent->name] = meshPart;
	}

	for(unsigned int i = 0; i < assimpNode->mNumChildren; i++)
	{
		aiNode* childNode = assimpNode->mChildren[i];
		nodePart->nodeChildren.push_back(nodeLoop(childNode, indent + 1, nodePart));
	}
	return nodePart;
}

Animation* ModelAsset::FindAnim(std::string findThis)
{
	std::map<std::string, Animation*>::iterator t = animations.find(findThis);
	if(t != animations.end())
		return t->second;
	return nullptr;
}
