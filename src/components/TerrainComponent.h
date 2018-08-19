//
// Created by Tim on 16/08/2018.
//

#ifndef BLOCKHOP_TERRAINCOMPONENT_H
#define BLOCKHOP_TERRAINCOMPONENT_H

#include "../ecs/component.h"
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include <GL/glew.h>

struct TerrainVertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class TerrainComponent : public Component<TerrainComponent>
{
public:
	TerrainComponent();
	~TerrainComponent();
	void setValues(json inValues);
	void genBuffers();
	void createVAO();

	float width;
	float depth;
	float lowerHeight;
	float upperHeight;
	std::string filename;

	std::vector<TerrainVertex> vertices;
	std::vector<unsigned int> indices;

	GLuint texture;

	GLuint VAO;
	GLuint vertexBuffer;
	GLuint indexBuffer;

	friend std::ostream &operator<< (std::ostream &os, TerrainComponent const &c) {
		os << "TerrainComponent";
		return os;
	}
};

#endif //BLOCKHOP_TERRAINCOMPONENT_H