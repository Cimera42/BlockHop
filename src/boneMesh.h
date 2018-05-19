#ifndef BONEMESH_H_INCLUDED
#define BONEMESH_H_INCLUDED

#include <vector>
#include <assimp/scene.h>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <GL/glew.h>
#include "mesh.h"
#include "components/AnimatedModelComponent.h"

struct BoneVertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
	int materialIndex;
	int BoneIds[4] = {0,0,0,0};
	float BoneWeights[4] = {0,0,0,0};
};

struct Bone
{
	int id;
	std::string name;
	glm::mat4 offsetMatrix;
};

class NodePart;
class BoneMesh : public Mesh
{
	public:
		BoneMesh(std::string inName, aiMesh *assimpMesh, std::map<std::string, aiNode *> nodes);
		~BoneMesh();

		std::vector<Bone*> bones;
		std::vector<BoneVertex> collatedVertices;

		void createVAO() override;
		void genBuffers() override;
		void load(aiMesh *assimpMesh) override;
		void loadWithVectors(std::vector<glm::vec3> inVertices,
		                     std::vector<glm::vec2> inUvs,
		                     std::vector<glm::vec3> inNormals,
		                     std::vector<unsigned int> inIndices) override;

		NodePart* FindNode(std::map<std::string, NodePart *> nodes, std::string findThis);
};

#endif // BONEMESH_H_INCLUDED
