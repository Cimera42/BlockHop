//
// Created by Tim on 24/06/2017.
//

#include "PhysicsComponent.h"
#include "../ecs/ecsManager.h"

#include <bullet3/btBulletCollisionCommon.h>

COMPONENT_EXPORT(PhysicsComponent, "physicsComponent")

PhysicsComponent::PhysicsComponent() {}
PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::setValues(json inValues)
{
	std::string shape = inValues["colliderShape"];
	if(shape == "cube")
	{
		collisionShapeBt = new btBoxShape(
			btVector3(
				inValues["halfWidth"],
				inValues["halfHeight"],
				inValues["halfDepth"]
			)
		);
	}

	mass = inValues["mass"];

	auto jointDataJson = inValues.find("joints");
	if(jointDataJson != inValues.end())
		jointData = (*jointDataJson).get<std::vector<json> >();
}
