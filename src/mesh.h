#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <vector>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include "shader.h"

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
	int materialIndex;
};

class Mesh
{
public:
	explicit Mesh(const std::string &inName);
	Mesh(const std::string &inName, aiMesh *assimpMesh);
	void destruct();

	std::string name;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> materialIndices;

	std::vector<Vertex> collatedVertices;
	std::vector<unsigned int> indices;

	GLuint VAO = 0;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	virtual void createVAO();
	virtual void genBuffers();

	virtual void load(aiMesh* assimpMesh);
	virtual void loadWithVectors(std::vector<glm::vec3> inVertices,
	                             std::vector<glm::vec2> inUvs,
	                             std::vector<glm::vec3> inNormals,
	                             std::vector<unsigned int> inIndices);
};

#endif // MESH_H_INCLUDED
