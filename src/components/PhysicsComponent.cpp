//
// Created by Tim on 24/06/2017.
//

#include "PhysicsComponent.h"
#include "../ecs/ecsManager.h"
#include <reactphysics3d.h>

COMPONENT_EXPORT(PhysicsComponent, "physicsComponent")

PhysicsComponent::PhysicsComponent() {}
PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::setValues(json inValues) 
{
	std::string shape = inValues["colliderShape"].get<std::string>();
	if(shape == "cube")
	{
		collisionShape = new rp3d::BoxShape(rp3d::Vector3(inValues["halfWidth"].get<float>(),
														  inValues["halfHeight"].get<float>(),
														  inValues["halfDepth"].get<float>()));
	}
	else if(shape == "sphere")
	{
		collisionShape = new rp3d::SphereShape(inValues["radius"].get<float>());
	}
	else if(shape == "cone")
	{
		collisionShape = new rp3d::ConeShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
	}
	else if(shape == "cylinder")
	{
		collisionShape = new rp3d::CylinderShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
	}
	else if(shape == "cylinder")
	{
		collisionShape = new rp3d::CapsuleShape(inValues["radius"].get<float>(), inValues["height"].get<float>());
	}

	std::string mode = inValues["mode"].get<std::string>();
	if(mode == "static")
	{
		collisionMode = rp3d::STATIC;
	}
	else if(mode == "kinematic")
	{
		collisionMode = rp3d::KINEMATIC;
	}
	else if(mode == "dynamic")
	{
		collisionMode = rp3d::DYNAMIC;
	}
}

reactphysics3d::CollisionShape *PhysicsComponent::getCollisionShape() const
{
	return collisionShape;
}

reactphysics3d::BodyType PhysicsComponent::getMode() const
{
	return collisionMode;
}
