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
	for(auto entity : getEntities())
	{
		CameraComponent* camera = entity->getComponent<CameraComponent>("cameraComponent");
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
		
		camera->lookAt(transform->getPosition(),
					   transform->getPosition() + transform->getForward(),
					   transform->getUp());
	}
}