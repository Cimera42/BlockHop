//
// Created by Jono on 07/07/2018.
//

#include "EventSystem.h"
#include "../ecs/ecsManager.h"

SYSTEM_EXPORT(EventSystem, "eventSystem")

EventSystem::EventSystem() {}
EventSystem::~EventSystem() {}

void EventSystem::update(double dt)
{
	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		//XComponent* x = entity->getComponent<XComponent>();
	}
}