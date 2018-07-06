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
	std::string shape = inValues["colliderShape"].get<std::string>();
	if(shape == "cube")
	{
		collisionShapeBt = new btBoxShape(
			btVector3(
				inValues["halfWidth"].get<float>(),
				inValues["halfHeight"].get<float>(),
				inValues["halfDepth"].get<float>()
			)
		);
	}

	mass = inValues["mass"].get<float>();
//	else if(shape == "sphere")
//	{
//		collisionShape = new rp3d::SphereShape(inValues["radius"].get<float>());
//	}
//	else if(shape == "cone")
//	{
//		collisionShape = new rp3d::ConeShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
//	}
//	else if(shape == "cylinder")
//	{
//		collisionShape = new rp3d::CylinderShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
//	}
//	else if(shape == "capsule")
//	{
//		collisionShape = new rp3d::CapsuleShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
//	}

	auto jointDataJson = inValues.find("joints");
	if(jointDataJson != inValues.end())
		jointData = (*jointDataJson).get<std::vector<json> >();
}
