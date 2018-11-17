//
// Created by Tim on 17/11/2018.
//

#include "TransformSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

SYSTEM_EXPORT(TransformSystem, "transformSystem")

TransformSystem::TransformSystem() {}
TransformSystem::~TransformSystem() {}

void TransformSystem::update(double dt, double alpha)
{
	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		auto physicsComponent = entity->getComponent<PhysicsComponent>();
		if(!physicsComponent)
		{
			auto transformComponent = entity->getComponent<TransformComponent>();
			transformComponent->setPosition(transformComponent->getAlphaPosition(dt));
			transformComponent->setRotation(transformComponent->getAlphaRotation(dt));
		}
	}
}