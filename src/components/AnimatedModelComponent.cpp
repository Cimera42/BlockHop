//
// Created by Tim on 25/05/2017.
//

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "AnimatedModelComponent.h"
#include "../ecs/ecsManager.h"
#include "../openGLFunctions.h"
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
	for(auto nM : normalMeshes)
		nM.second->destruct();
	for(auto bM : boneMeshes)
		bM.second->destruct();
}

void AnimatedModelComponent::setValues(json inValues) {
    //Will throw if incorrect/should automatically be caught by ECSManager

	filename = inValues["filename"].get<std::string>();
	load();
}

void AnimatedModelComponent::load()
{
	Assimp::Importer importer;
	Logger(1) << "Loading model: " << filename;
	const aiScene* scene = importer.ReadFile(filename,
											 aiProcess_CalcTangentSpace |
											 aiProcess_Triangulate |
											 aiProcess_JoinIdenticalVertices |
											 aiProcess_SortByPType |
											 aiProcess_LimitBoneWeights |
											 aiProcess_ValidateDataStructure// |
                                             //aiProcess_PreTransformVertices
	);

	if(!scene)
	{
		Logger(1) << importer.GetErrorString();
		Logger(1) << "Could not load Mesh. Error importing";
		return;
	}

	Logger(1) << "Animations: " << scene->mNumAnimations;
	for(unsigned int i = 1; i < scene->mNumAnimations; i++)
	{
		aiAnimation* assimpAnimation = scene->mAnimations[i];
		Logger(1) << "TPS: " << assimpAnimation->mTicksPerSecond;

		Logger(1) << "Animation name: " << assimpAnimation->mName.C_Str();
		Logger(1) << "Animation channels: " << assimpAnimation->mNumChannels;
		for(unsigned int j = 0; j < assimpAnimation->mNumChannels; j++)
		{
			animNodes.push_back(assimpAnimation->mChannels[j]);
		}
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
			material.texturePath = std::string(texPath.C_Str());
		}

		materials.push_back(material);
	}

	std::vector<std::string> texPaths;
	for(unsigned int i = 0; i < materials.size(); i++)
	{
		if(materials[i].texturePath.find_first_not_of(' ') != std::string::npos)
		{
			std::string backslashFixed = materials[i].texturePath;
			std::replace(backslashFixed.begin(), backslashFixed.end(), '\\', '/');

			std::string baseFolder = std::string(filename);
			baseFolder = baseFolder.substr(0, baseFolder.find_last_of("/"));

			texPaths.push_back((baseFolder +"/"+ backslashFixed).c_str());
		}
	}
	if(texPaths.size() > 0)
	{
		texture.load(texPaths);
	}

	nodeLoop(scene->mRootNode, 0, nullptr);
	transformNodes();

	for(unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* assimpMesh = scene->mMeshes[i];
		if(assimpMesh->mNumBones > 0)
		{
			boneMeshes[i] = new BoneMesh(assimpMesh, assimpNodes);
			boneMeshes[i]->transformBones(nodeParts);
		}
		else
		{
			normalMeshes[i] = new Mesh(assimpMesh);
		}
	}
}

NodePart* AnimatedModelComponent::nodeLoop(aiNode *assimpNode, int indent, NodePart *parent)
{
	assimpNodes.push_back(assimpNode);

	glm::mat4 nodeMatrix = AToGMat(assimpNode->mTransformation);
	
	NodePart* nodePart = new NodePart();
	nodePart->name = assimpNode->mName.C_Str();
	nodePart->animationNode = new AnimationNode(FindAnimNode(nodePart->name), assimpNode);
	nodePart->nodeParent = parent;
    //nodePart->defaultTransform = AToGMat(assimpNode->mTransformation);

//	glm::vec3 skew;
//	glm::vec4 perspective;
//	glm::decompose(nodePart->defaultTransform, scale, rotation, position, skew, perspective);

    /*
     * Extract scale, rotation and position
     * Then use them to cosntruct a transformation matrix
     * This could also be done using AToGMat, which directly converts the matrix
     * The component extarction is done anyway for logging, so why not
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
    //rotation = glm::quat(0,707,0,0,0.707);
    scale = glm::vec3(s.x, s.y, s.z);
    nodePart->defaultTransform = glm::mat4() * glm::translate(position) * glm::mat4_cast(rotation) * glm::scale(scale);

    std::string indentS = [indent](){std::string c; for(int i = 0; i < indent;i++){c+="    ";} return c;}();
    Logger(1) << indentS << "Node: \"" << nodePart->name << "\"";
    Logger(1) << indentS << "    Position: " << position;
    Logger(1) << indentS << "    Rotation: " << rotation;
    Logger(1) << indentS << "    RotationA: " << glm::axis(rotation);
    Logger(1) << indentS << "    Scale: " << scale;

	nodeParts.push_back(nodePart);
	
	for(int i = 0; i < assimpNode->mNumMeshes; i++)
	{
		MeshPart* meshPart = new MeshPart();
		meshPart->mesh = assimpNode->mMeshes[i];
		meshPart->nodeParent = nodePart;
		meshParts.push_back(meshPart);
	}

	for(unsigned int i = 0; i < assimpNode->mNumChildren; i++)
	{
		aiNode* childNode = assimpNode->mChildren[i];
		nodePart->nodeChildren.push_back(nodeLoop(childNode, indent + 1, nodePart));
	}
	return nodePart;
}

aiNodeAnim* AnimatedModelComponent::FindAnimNode(std::string findThis)
{
	for(unsigned int i = 0; i < animNodes.size(); i++)
	{
		if(animNodes[i]->mNodeName.C_Str() == findThis)
			return animNodes[i];
	}
	return nullptr;
}

void AnimatedModelComponent::transformNodes()
{
	float time = 0;
	for(unsigned int i = 1; i < nodeParts.size(); i++)
	{
		NodePart* node = nodeParts[i];

		glm::mat4 transform;
		glm::vec3 position = node->InterpolatePosition(time);
		glm::quat rotation = node->InterpolateRotation(time);
		glm::vec3 scale = node->InterpolateScaling(time);
		transform *= glm::translate(position);
		transform *= glm::mat4_cast(rotation);
		transform *= glm::scale(scale);
//		if(!node->nodeParent)
//			node->localMatrix = glm::inverse(node->defaultTransform);
//		else
		if(node->name != "Armature")
			node->localMatrix = node->defaultTransform;
		else
			node->localMatrix = glm::mat4();
	}
	recursiveTransform(nodeParts[0]);
}

void AnimatedModelComponent::recursiveTransform(NodePart *node)
{
	node->collectiveMatrix = node->localMatrix;
	if(node->nodeParent)
		node->collectiveMatrix *= node->nodeParent->collectiveMatrix;

	for(auto nodeChildren : node->nodeChildren)
		recursiveTransform(nodeChildren);
}

unsigned int NodePart::PositionIndex(float time)
{
	for(unsigned int i = 0; i < animationNode->mNumPositionKeys-1; i++)
	{
		if(time > animationNode->mPositionKeys[i].mTime)
			if(time < animationNode->mPositionKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::vec3 NodePart::InterpolatePosition(float time)
{
	if(animationNode->mNumPositionKeys < 1)
		return glm::vec3(0,0,0);
	if(animationNode->mNumPositionKeys == 1)
		return animationNode->mPositionKeys[0].mValue;

	unsigned int nodeIndex = PositionIndex(time);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::vec3 nodePosition = animationNode->mPositionKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mPositionKeys[nodeIndex].mTime;
	glm::vec3 nextNodePosition = animationNode->mPositionKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mPositionKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodePosition, nextNodePosition, between);
}
unsigned int NodePart::RotationIndex(float time)
{
	for(unsigned int i = 0; i < animationNode->mNumRotationKeys-1; i++)
	{
		if(time > animationNode->mRotationKeys[i].mTime)
			if(time < animationNode->mRotationKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::quat NodePart::InterpolateRotation(float time)
{
	if(animationNode->mNumRotationKeys < 1)
		return glm::quat(1,0,0,0);
	if(animationNode->mNumRotationKeys == 1)
		return animationNode->mRotationKeys[0].mValue;

	unsigned int nodeIndex = RotationIndex(time);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::quat nodeRotation = animationNode->mRotationKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mRotationKeys[nodeIndex].mTime;
	glm::quat nextNodeRotation = animationNode->mRotationKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mRotationKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodeRotation, nextNodeRotation, between);
}
unsigned int NodePart::ScalingIndex(float time)
{
	for(unsigned int i = 0; i < animationNode->mNumScalingKeys-1; i++)
	{
		if(time > animationNode->mScalingKeys[i].mTime)
			if(time < animationNode->mScalingKeys[i+1].mTime)
				return i;
	}
	return 0;
}
glm::vec3 NodePart::InterpolateScaling(float time)
{
	if(animationNode->mNumScalingKeys < 1)
		return glm::vec3(1,1,1);
	if(animationNode->mNumScalingKeys == 1)
		return animationNode->mScalingKeys[0].mValue;

	unsigned int nodeIndex = ScalingIndex(time);
	unsigned int nextNodeIndex = nodeIndex+1;

	glm::vec3 nodeScaling = animationNode->mScalingKeys[nodeIndex].mValue;
	float nodeTime = animationNode->mScalingKeys[nodeIndex].mTime;
	glm::vec3 nextNodeScaling = animationNode->mScalingKeys[nextNodeIndex].mValue;
	float nextNodeTime = animationNode->mScalingKeys[nextNodeIndex].mTime;

	float between = glm::clamp((time - nodeTime)/(nextNodeTime - nodeTime),0.0f,1.0f);

	return glm::mix(nodeScaling, nextNodeScaling, between);
}

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
		key.mTime = animNode->mPositionKeys[i].mTime;
		mPositionKeys.push_back(key);
	}

	for(int i = 0; i < mNumRotationKeys; i++)
	{
		aiQuaternion p = animNode->mRotationKeys[i].mValue;
		QuatKey key;
		key.mValue = glm::quat(p.w,p.x,p.y,p.z);
		key.mTime = animNode->mRotationKeys[i].mTime;
		mRotationKeys.push_back(key);
	}

	for(int i = 0; i < mNumScalingKeys; i++)
	{
		aiVector3D p = animNode->mScalingKeys[i].mValue;
		VectorKey key;
		key.mValue = glm::vec3(p.x,p.y,p.z);
		key.mTime = animNode->mScalingKeys[i].mTime;
		mScalingKeys.push_back(key);
	}
}
