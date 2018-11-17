//
// Created by Tim on 19/04/2017.
//

#include "TransformComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

COMPONENT_EXPORT(TransformComponent, "transformComponent")

TransformComponent::TransformComponent() {}
TransformComponent::~TransformComponent() {}

void TransformComponent::setValues(json inValues) {
	//Will throw if incorrect/should automatically be caught by ECSManager
	position = inValues["position"];

	rotation = inValues["rotation"];
	rotation = glm::normalize(rotation);

	scale = inValues["scale"];

	velocity = glm::vec3(0,0,0);
	angularVelocity = glm::quat(0,0,0,0);

	genMatrix();
	genVectors();
}

void TransformComponent::genMatrix()
{
	matrix = glm::mat4();
	matrix *= glm::translate(position);
	matrix *= glm::toMat4(rotation);
	matrix *= glm::scale(scale);
}

void TransformComponent::genVectors()
{
	forward = glm::vec3(0,0,1) * rotation;
	right = glm::vec3(1,0,0) * rotation;
	up = glm::vec3(0,1,0) * rotation;
}

glm::vec3 TransformComponent::getPosition() const { return position; }
glm::vec3 TransformComponent::getAlphaPosition(const double alpha) const
{
	return position + (velocity * float(alpha));
}
void TransformComponent::setPosition(glm::vec3 inPosition)
{
	position = inPosition;
	genMatrix();
}

glm::quat TransformComponent::getRotation() const { return rotation; }
glm::quat TransformComponent::getAlphaRotation(const double alpha) const
{
	return rotation + ((float(alpha) * angularVelocity * rotation) / 2.0f);
}
void TransformComponent::setRotation(glm::quat inRotation)
{
	rotation = inRotation;
	genVectors();
	genMatrix();
}

glm::vec3 TransformComponent::getScale() const { return scale; }
void TransformComponent::setScale(glm::vec3 inScale)
{
	scale = inScale;
	genMatrix();
}

glm::vec3 TransformComponent::getVelocity() const { return velocity; }
void TransformComponent::setVelocity(glm::vec3 inVelocity)
{
	velocity = inVelocity;
}

glm::quat TransformComponent::getAngularVelocity() const { return angularVelocity; }
void TransformComponent::setAngularVelocity(glm::quat inAngularVelocity)
{
	angularVelocity = inAngularVelocity;
}

glm::mat4 TransformComponent::getMatrix() const { return matrix; }
glm::vec3 TransformComponent::getForward() const { return forward; }
glm::vec3 TransformComponent::getRight() const { return right; }
glm::vec3 TransformComponent::getUp() const { return up; }
