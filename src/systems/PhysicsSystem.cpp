//
// Created by Tim on 24/06/2017.
//

#include "PhysicsSystem.h"
#include "../ecs/ecsManager.h"
#include "../logger.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"

#include <reactphysics3d.h>

SYSTEM_EXPORT(PhysicsSystem, "physicsSystem")

PhysicsSystem::PhysicsSystem() 
{
	accumulator = 0;
	
	rp3d::Vector3 gravity(0,-9.81f,0);
	dynamicsWorld = new rp3d::DynamicsWorld(gravity);
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
	
	rigidBody->addCollisionShape(physicsComp->getCollisionShape(), rp3d::Transform::identity(), 1.0f);
	rigidBody->setType(physicsComp->getMode());
}

void PhysicsSystem::update(double dt)
{
	accumulator += dt;
	
	while(accumulator >= idealTimestep)
	{
		dynamicsWorld->update(idealTimestep);
		
		accumulator -= idealTimestep;
	}
    for(auto entity : getEntities())
    {
        TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
		 
		rp3d::RigidBody* rb = findRigidBody(entity);
		if(rb)
		{
			rp3d::Vector3 pos = rb->getTransform().getPosition();
			rp3d::Quaternion rot = rb->getTransform().getOrientation();
			transform->setPosition(glm::vec3(pos.x, pos.y, pos.z));
			transform->setRotation(glm::quat(rot.w, rot.x, rot.y, rot.z));
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
