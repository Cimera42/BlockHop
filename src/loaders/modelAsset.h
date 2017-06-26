//
// Created by Tim on 13/06/2017.
//

#ifndef BLOCKHOP_MODELASSET_H
#define BLOCKHOP_MODELASSET_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>
#include <assimp/anim.h>
#include "imageAsset.h"

class Mesh;
class BoneMesh;

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

struct Animation
{
	std::string name;
	unsigned int channels;
	double tickRate;
	double duration;

	std::map<std::string, AnimationNode*> animationNodes;
	std::map<std::string, aiNodeAnim*> animNodes;
	
	aiNodeAnim* FindAnimNode(std::string findThis);
};

struct NodePart
{
	glm::mat4 defaultTransform;

	unsigned int PositionIndex(float time, Animation* animation);
	glm::vec3 InterpolatePosition(float time, Animation* animation);

	unsigned int RotationIndex(float time, Animation* animation);
	glm::quat InterpolateRotation(float time, Animation* animation);

	unsigned int ScalingIndex(float time, Animation* animation);
	glm::vec3 InterpolateScaling(float time, Animation* animation);

	std::string name;
	NodePart* nodeParent;
	std::vector<NodePart*> nodeChildren;
};

struct MeshPart
{
	unsigned int mesh;

	NodePart* nodeParent;
};

struct Material
{
	ImageAsset* image;
};

struct BoneMesh;
class ModelAsset : public BaseAsset
{
    friend class ModelLoader;
	bool load();
    ModelAsset(std::string inFilename);
    ~ModelAsset();
public:
	NodePart* nodeLoop(aiNode *assimpNode, int indent, NodePart *parent);
	Animation* FindAnim(std::string findThis);

	std::string rootNodeName;
	std::vector<Material> materials;
	GLuint texture;

	std::map<unsigned int, Mesh*> normalMeshes;
	std::map<unsigned int, BoneMesh*> boneMeshes;
	std::map<std::string, MeshPart*> meshParts;
	std::map<std::string, NodePart*> nodeParts;

	std::map<std::string, aiNode*> assimpNodes;
	std::map<std::string, Animation*> animations;
};


#endif //BLOCKHOP_MODELASSET_H
