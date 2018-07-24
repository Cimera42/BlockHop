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
	for(auto trig : getTriggers()) {
		trig->runSystemFunction(this);
	}
	
	for(auto entity : getEntities())
	{
		for(auto entTrig : entity->getTriggers()) {
			if(entTrig->getSystemName() == getName()) {
				entTrig->runEntityCheck(this, entity);
			}
		}
		
		//XComponent* x = entity->getComponent<XComponent>("xComponent");
	}
}