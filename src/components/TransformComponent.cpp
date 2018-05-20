//
// Created by Tim on 19/04/2017.
//
#include "TransformComponent.h"
#include "../ecs/ecsManager.h"
#include "../utils.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

COMPONENT_EXPORT(TransformComponent, "transformComponent")
void TransformComponent::setValues(const json &inValues) {
	//Will throw if incorrect/should automatically be caught by ECSManager
	position.x = inValues["position"]["x"].get<float>();
	position.y = inValues["position"]["y"].get<float>();
	position.z = inValues["position"]["z"].get<float>();

	rotation.w = inValues["rotation"]["w"].get<float>();
	rotation.x = inValues["rotation"]["x"].get<float>();
	rotation.y = inValues["rotation"]["y"].get<float>();
	rotation.z = inValues["rotation"]["z"].get<float>();

	scale.x = inValues["scale"]["x"].get<float>();
	scale.y = inValues["scale"]["y"].get<float>();
	scale.z = inValues["scale"]["z"].get<float>();

	genMatrix();
	genVectors();
}

void TransformComponent::genMatrix()
{
	matrix = glm::mat4();
	matrix *= glm::translate(position);
	matrix *= glm::mat4_cast(rotation);
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

std::string TransformComponent::serialise()
{
	std::string serialString;
	serialString += getName();
	serialString += ":";
	for(int i = 0; i < position.length(); i++)
	{
		if(i != 0) serialString += ",";
		serialString += std::to_string(position[i]);
	}
	serialString += "|";
	for(int i = 0; i < rotation.length(); i++)
	{
		if(i != 0) serialString += ",";
		serialString += std::to_string(rotation[i]);
	}
	serialString += "|";
	for(int i = 0; i < 3; i++)
	{
		if(i != 0) serialString += ",";
		serialString += std::to_string(scale[i]);
	}
	return serialString;
}

void TransformComponent::deserialise(const std::string &data)
{
	std::vector<std::string> p1 = splitString(data, ':');
	std::string name = p1[0];
	std::vector<std::string> p2 = splitString(p1[1], '|');

	std::vector<std::string> posParts = splitString(p2[0], ',');
	position = glm::vec3(
		std::stof(posParts[0]),
		std::stof(posParts[1]),
		std::stof(posParts[2])
	);
	std::vector<std::string> rotParts = splitString(p2[1], ',');
	rotation = glm::quat(
			std::stof(rotParts[0]),
			std::stof(rotParts[1]),
			std::stof(rotParts[2]),
			std::stof(rotParts[3])
	);
	std::vector<std::string> scaParts = splitString(p2[2], ',');
	scale = glm::vec3(
			std::stof(scaParts[0]),
			std::stof(scaParts[1]),
			std::stof(scaParts[2])
	);

	genVectors();
	genMatrix();
}
