//
// Created by Tim on 24/06/2017.
//

#ifndef BLOCKHOP_PHYSICSCOMPONENT_H
#define BLOCKHOP_PHYSICSCOMPONENT_H

#include "../ecs/component.h"
#include <reactphysics3d.h>

class PhysicsComponent : public ComponentStatics<PhysicsComponent>
{
	rp3d::CollisionShape* collisionShape;
	rp3d::ProxyShape* collisionShapeInstance;
	rp3d::BodyType collisionMode;

public:
	PhysicsComponent();
	~PhysicsComponent();
	void setValues(json inValues);

	rp3d::CollisionShape *getCollisionShape() const;
	void setCollisionShapeInstance(rp3d::ProxyShape *collisionShapeInstance);
	rp3d::ProxyShape *getCollisionShapeInstance() const;
	rp3d::BodyType getMode() const;

	friend std::ostream &operator<< (std::ostream &os, PhysicsComponent const &c) {
		os << "PhysicsComponent";
		return os;
	}
};

#endif //BLOCKHOP_PHYSICSCOMPONENT_H