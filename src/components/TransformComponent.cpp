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
void TransformComponent::setPosition(glm::vec3 inPosition)
{
	position = inPosition;
	genMatrix();
}

glm::quat TransformComponent::getRotation() const { return rotation; }
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

glm::mat4 TransformComponent::getMatrix() const { return matrix; }
glm::vec3 TransformComponent::getForward() const { return forward; }
glm::vec3 TransformComponent::getRight() const { return right; }
glm::vec3 TransformComponent::getUp() const { return up; }
