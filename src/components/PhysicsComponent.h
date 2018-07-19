//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSCOMPONENT_H
#define BLOCKHOP_PHYSICSCOMPONENT_H

#include "../ecs/component.h"
#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsComponent : public Component
{
public:
	btCollisionShape* collisionShape;
	static bool exported;

	PhysicsComponent();
	~PhysicsComponent();
	void setValues(json inValues);

	float mass;

	std::vector<json> jointData;

	friend std::ostream &operator<< (std::ostream &os, PhysicsComponent const &c) {
		os << "PhysicsComponent";
		return os;
	}
};

#endif //BLOCKHOP_PHYSICSCOMPONENT_H