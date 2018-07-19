//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSSYSTEM_H
#define BLOCKHOP_PHYSICSSYSTEM_H

#include "../ecs/system.h"

#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsSystem : public System
{
	static bool exported;


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

	btRigidBody *findRigidBody(Entity *toFind);
};

#endif //BLOCKHOP_PHYSICSSYSTEM_H