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

public:
	btDiscreteDynamicsWorld* dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btDbvtBroadphase* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	std::map<Entity*, btRigidBody*> rigidBodies;

	PhysicsSystem();
	~PhysicsSystem() override;

	void subscribeCallback(Entity* entSubbed) override;

	void update(double dt, double alpha) override;

	btTypedConstraint *
	makeJoint(json jointData, btRigidBody *rigidBody, Entity *entSubbed, PhysicsComponent *physicsComp);
	btRigidBody *findRigidBody(Entity *toFind);
};

#endif //BLOCKHOP_PHYSICSSYSTEM_H