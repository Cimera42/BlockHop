//
// Created by Tim on 24/06/2017.
//

#include "PhysicsSystem.h"
#include "../ecs/ecsManager.h"
#include "../logger.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../window.h"
#include "MouseButtonSystem.h"

#include <reactphysics3d.h>


SYSTEM_EXPORT(PhysicsSystem, "physicsSystem")

PhysicsSystem::PhysicsSystem() 
{
	accumulator = 0;
	
	rp3d::Vector3 gravity(0,-9.81f,0);
	dynamicsWorld = new rp3d::DynamicsWorld(gravity);
	//dynamicsWorld->enableSleeping(false);
}
PhysicsSystem::~PhysicsSystem() 
{
//	dynamicsWorld.destroyCollisionBody(collisionBody);
	delete dynamicsWorld;
}

void PhysicsSystem::subscribeCallback(Entity *entSubbed)
{
	TransformComponent* transformComp = entSubbed->getComponent<TransformComponent>("transformComponent");
	PhysicsComponent* physicsComp = entSubbed->getComponent<PhysicsComponent>("physicsComponent");
	glm::vec3 startPos = transformComp->getPosition();
	
	rp3d::Vector3 initPos(startPos.x,startPos.y,startPos.z);
	rp3d::Quaternion initRot = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPos, initRot);
	rp3d::RigidBody* rigidBody;
	
	rigidBody = dynamicsWorld->createRigidBody(transform);
	rigidBodies[entSubbed] = rigidBody;
	
	rp3d::ProxyShape* proxyShape = rigidBody->addCollisionShape(physicsComp->getCollisionShape(), rp3d::Transform::identity(), 1.0f);
	physicsComp->setCollisionShapeInstance(proxyShape);
	rigidBody->setType(physicsComp->getMode());
}

extern Window window;
void PhysicsSystem::update(double dt)
{
	accumulator += dt;
	
	while(accumulator >= idealTimestep)
	{
		dynamicsWorld->update(idealTimestep);
		
		accumulator -= idealTimestep;
	}

	//Run triggers
	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		//Check if entity has triggers that are in this system
        updateEntityTriggers(entity);

		TransformComponent* transformComp = entity->getComponent<TransformComponent>("transformComponent");
		 
		rp3d::RigidBody* rb = findRigidBody(entity);
		if(rb)
		{
			if(rb->getType() == rp3d::DYNAMIC)
			{
				rp3d::Vector3 pos = rb->getTransform().getPosition();
				rp3d::Quaternion rot = rb->getTransform().getOrientation();
				transformComp->setPosition(glm::vec3(pos.x, pos.y, pos.z));
				transformComp->setRotation(glm::quat(rot.w, rot.x, rot.y, rot.z));
			}
		}
	}
}

rp3d::RigidBody* PhysicsSystem::findRigidBody(Entity* toFind)
{
	auto t = rigidBodies.find(toFind);
	if(t != rigidBodies.end())
		return t->second;
	return nullptr;
}
