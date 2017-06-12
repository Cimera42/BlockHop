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

struct Animation
{
	std::string name;
	unsigned int channels;
	double tickRate;
	double duration;

	std::map<std::string, AnimationNode*> animationNodes;
	std::map<std::string, aiNodeAnim*> animNodes;
};

struct NodePart
{
	glm::mat4 defaultTransform;
	glm::mat4 localMatrix;
	glm::mat4 collectiveMatrix;
	
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
	std::vector<Animation*> animations;
	
	double time = 0;
	int currentAnimation = 0;

	void load();
	NodePart* nodeLoop(aiNode *assimpNode, int indent, NodePart *parent);
	int FindAnim(std::string findThis);
	aiNodeAnim* FindAnimNode(std::string findThis, Animation* anim);
	
    friend std::ostream &operator<< (std::ostream &os, AnimatedModelComponent const &c) {
        os << "AnimatedModelComponent: ";
        os << c.filename;
        return os;
    }
	
	void transformNodes(float dt);
	void recursiveTransform(NodePart *node);

	bool playAnimation(std::string name);
	bool playAnimation(int index);
};

#endif //BLOCKHOP_BONEMODELCOMPONENT_H
