//
// Created by Jono on 10/07/2018.
//

#include "TeamComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(TeamComponent, "teamComponent")

TeamComponent::TeamComponent() {}
TeamComponent::~TeamComponent() {}

void TeamComponent::setValues(json inValues) 
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	team = inValues["team"].get<int>();
}