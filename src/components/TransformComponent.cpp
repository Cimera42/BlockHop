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
	angularVelocity = glm::vec3(0,0,0);

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
	auto quatAV = glm::quat(0, angularVelocity.x, angularVelocity.y, angularVelocity.z);
	auto r = rotation + ((float(alpha) * quatAV * rotation) / 2.0f);
	return glm::normalize(r);
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

glm::vec3 TransformComponent::getAngularVelocity() const { return angularVelocity; }
void TransformComponent::setAngularVelocity(glm::vec3 inAngularVelocity)
{
	angularVelocity = inAngularVelocity;
}

glm::mat4 TransformComponent::getMatrix() const { return matrix; }
glm::vec3 TransformComponent::getForward() const { return forward; }
glm::vec3 TransformComponent::getRight() const { return right; }
glm::vec3 TransformComponent::getUp() const { return up; }

glm::mat4 TransformComponent::getAlphaMatrix(const double alpha) const
{
	auto alphaMatrix = glm::mat4();
	alphaMatrix *= glm::translate(getAlphaPosition(alpha));
	alphaMatrix *= glm::toMat4(getAlphaRotation(alpha));
	alphaMatrix *= glm::scale(scale);
	return alphaMatrix;
}

glm::vec3 TransformComponent::getAlphaForward(const double alpha) const
{
	return glm::vec3(0,0,1) * getAlphaRotation(alpha);
}
glm::vec3 TransformComponent::getAlphaRight(const double alpha) const
{
	return glm::vec3(1,0,0) * getAlphaRotation(alpha);
}
glm::vec3 TransformComponent::getAlphaUp(const double alpha) const
{
	return glm::vec3(0,1,0) * getAlphaRotation(alpha);
}