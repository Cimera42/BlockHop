//
// Created by Tim on 19/04/2017.
//

#ifndef BLOCKHOP_TRANSFORMCOMPONENT_H
#define BLOCKHOP_TRANSFORMCOMPONENT_H

#include "../ecs/component.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <ostream>
#include <string>

class TransformComponent : public Component
{
	static bool exported;

	glm::mat4 matrix;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	void genMatrix();
	void genVectors();

public:
	void setValues(const json &inValues) override;

	glm::mat4 getMatrix() const;
	glm::vec3 getPosition() const;
	void setPosition(glm::vec3 inPosition);
	glm::quat getRotation() const;
	void setRotation(glm::quat inRotation);
	glm::vec3 getScale() const;
	void setScale(glm::vec3 inScale);

	glm::vec3 getForward() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;

	std::string serialise();
	void deserialise(const std::string &data);

	friend std::ostream &operator<< (std::ostream &os, TransformComponent const &t) {
		os << "TransformComponent: ";
		os << "(" << t.position.x << ", " << t.position.y << ", " << t.position.z << "), ";
		os << "(" << t.rotation.w << ", " << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << "), ";
		os << "(" << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << "), ";
		return os;
	}
};

#endif //BLOCKHOP_TRANSFORMCOMPONENT_H
