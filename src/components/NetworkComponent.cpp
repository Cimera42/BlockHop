//
// Created by Tim on 20/05/2018.
//

#include "NetworkComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(NetworkComponent, "networkComponent")

void NetworkComponent::setValues(const json &inValues)
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	ours = true;
}