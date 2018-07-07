//
// Created by Jono on 07/07/2018.
//

#include "TimeoutComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(TimeoutComponent, "timeoutComponent")

TimeoutComponent::TimeoutComponent() {}
TimeoutComponent::~TimeoutComponent() {}

void TimeoutComponent::setValues(json inValues) 
{
	//Will throw if incorrect/should automatically be caught by ECSManager

}