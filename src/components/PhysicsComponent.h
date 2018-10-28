//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSCOMPONENT_H
#define BLOCKHOP_PHYSICSCOMPONENT_H

#include "../ecs/component.h"
#include <bullet3/btBulletDynamicsCommon.h>

class PhysicsComponent : public Component<PhysicsComponent>
{
public:
	btCollisionShape* collisionShape;
	static bool exported;

	PhysicsComponent();
	~PhysicsComponent();
	void setValues(json inValues);
	btCollisionShape* loadShape(json inValues);

	float mass;
	btTransform principalTransform;
	btMotionState* motionState;
	btRigidBody* rigidBody;

	btVector3 offsetPos;
	btQuaternion offsetRot;

	json jsonData;
	std::vector<btTypedConstraint*> joints;

	std::vector<std::function<void()>> destructors;

	friend std::ostream &operator<< (std::ostream &os, PhysicsComponent const &c) {
		os << "PhysicsComponent";
		return os;
	}
};

#endif //BLOCKHOP_PHYSICSCOMPONENT_H