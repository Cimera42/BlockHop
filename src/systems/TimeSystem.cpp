//
// Created by Jono on 07/07/2018.
//

#include "TimeSystem.h"
#include "../ecs/ecsManager.h"

SYSTEM_EXPORT(TimeSystem, "timeSystem")

TimeSystem::TimeSystem() {}
TimeSystem::~TimeSystem() {}

void TimeSystem::update(double dt) 
{
	for(auto trig : getTriggers()) {
		trig->runSystemFunction(this);
	}
	
	for(auto entity : getEntities())
	{
		for(auto entTrig : entity->getTriggers()) {
			if(entTrig->getSystemName() == "timeSystem") {
				entTrig->runEntityCheck(this, entity);
			}
		}
		
		//XComponent* x = entity->getComponent<XComponent>("xComponent");
	}
}