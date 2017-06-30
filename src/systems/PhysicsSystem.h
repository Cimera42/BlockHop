//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSSYSTEM_H
#define BLOCKHOP_PHYSICSSYSTEM_H

#include "../ecs/system.h"

#include <reactphysics3d.h>

class PhysicsSystem : public System 
{
	static bool exported;
	
	rp3d::DynamicsWorld* dynamicsWorld;
	const float idealTimestep = 1.0f/60.0f;
	double accumulator;
	
	std::map<Entity*, rp3d::RigidBody*> rigidBodies;
	
public:
	PhysicsSystem();
	~PhysicsSystem();

	void subscribeCallback(Entity* entSubbed);
	
	void update(double dt);
	
	rp3d::RigidBody *findRigidBody(Entity *toFind);
};

#endif //BLOCKHOP_PHYSICSSYSTEM_H