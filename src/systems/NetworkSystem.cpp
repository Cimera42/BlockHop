//
// Created by Tim on 19/05/2018.
//

#include "NetworkSystem.h"
#include "../ecs/ecsManager.h"

SYSTEM_EXPORT(NetworkSystem, "networkSystem")

NetworkSystem::NetworkSystem() {}
NetworkSystem::~NetworkSystem() {}

void NetworkSystem::update(double dt)
{
	for(auto entity : getEntities())
	{

	}
}