//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSSYSTEM_H
#define BLOCKHOP_PHYSICSSYSTEM_H

#include "../ecs/system.h"
#include "../components/PhysicsComponent.h"

#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsSystem : public System<PhysicsSystem>
{
	const float idealTimestep = 1.0f/60.0f;
	double accumulator;

	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	std::map<Entity*, btRigidBody*> rigidBodies;

public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void subscribeCallback(Entity* entSubbed) override;

	void update(double dt) override;

	btTypedConstraint *
	makeJoint(json jointData, btRigidBody *rigidBody, Entity *entSubbed, PhysicsComponent *physicsComp);
	btRigidBody *findRigidBody(Entity *toFind);
};

#endif //BLOCKHOP_PHYSICSSYSTEM_H