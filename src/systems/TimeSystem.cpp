//
// Created by Jono on 07/07/2018.
//

#include "TimeSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/TimeoutComponent.h"

SYSTEM_EXPORT(TimeSystem, "timeSystem")

TimeSystem::TimeSystem() {}
TimeSystem::~TimeSystem() {}

void TimeSystem::update(double dt) 
{
	updateSystemTriggers();
	
	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		TimeoutComponent* timeout = entity->getComponent<TimeoutComponent>("timeoutComponent");
		if(timeout->hasTimedOut()) {
			timeout->runCallback(entity);
			entity->removeComponent("timeoutComponent");
		}
	}
}