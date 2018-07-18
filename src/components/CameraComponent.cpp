//
// Created by Tim on 25/05/2017.
//

#include <glm/gtc/matrix_transform.hpp>
#include "CameraComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(CameraComponent, "cameraComponent")

CameraComponent::CameraComponent() {}
CameraComponent::~CameraComponent() {}

void CameraComponent::setValues(json inValues)
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	fov = inValues["fov"];
	aspectRatio = inValues["aspectRatio"];
	nearDist = inValues["nearDist"];
	farDist = inValues["farDist"];

	perspective();
}

void CameraComponent::lookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
{
	viewMatrix = glm::lookAt(eye, centre, up);
}

void CameraComponent::perspective()
{
	projectionMatrix = glm::perspective(fov, aspectRatio, nearDist, farDist);
}

glm::mat4 CameraComponent::getViewMatrix() const { return viewMatrix; }
glm::mat4 CameraComponent::getProjectionMatrix() const { return projectionMatrix; }
