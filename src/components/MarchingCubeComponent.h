//
// Created by Tim on 12/09/2019.
//

#ifndef BLOCKHOP_MARCHINGCUBECOMPONENT_H
#define BLOCKHOP_MARCHINGCUBECOMPONENT_H

#include "../ecs/component.h"
#include <GL/glew.h>

class MarchingCubeComponent : public Component<MarchingCubeComponent>
{
public:
	MarchingCubeComponent();
	~MarchingCubeComponent();
	void setValues(json inValues);

	void genBuffers();
	void createVAO();

	std::vector<glm::vec3> triangles;
	std::vector<glm::vec3> normals;

	GLuint VAO;
	GLuint vertexBuffer;
	GLuint normalBuffer;

	friend std::ostream &operator<< (std::ostream &os, MarchingCubeComponent const &c) {
		os << "MarchingCubeComponent";
		return os;
	}
};

#endif //BLOCKHOP_MARCHINGCUBECOMPONENT_H