//
// Created by Tim on 25/05/2017.
//

#include "CameraSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/CameraComponent.h"
#include "../components/TransformComponent.h"

SYSTEM_EXPORT(CameraSystem, "cameraSystem")

CameraSystem::CameraSystem() {}
CameraSystem::~CameraSystem() {}

void CameraSystem::update(double dt)
{
	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		CameraComponent* camera = entity->getComponent<CameraComponent>();
		TransformComponent* transform = entity->getComponent<TransformComponent>();

		camera->lookAt(transform->getPosition(),
					   transform->getPosition() + transform->getForward(),
					   transform->getUp());
	}
}