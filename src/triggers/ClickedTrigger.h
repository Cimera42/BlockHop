//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_CLICKEDTRIGGER_H
#define BLOCKHOP_CLICKEDTRIGGER_H

#include "../ecs/trigger.h"
#include "../systems/PhysicsSystem.h"
#include <glm/glm.hpp>

class ClickedTrigger : public Trigger<ClickedTrigger> {
	void runBox2(SystemBase* sys, Entity* ent);
	void runBox(SystemBase* sys, Entity* ent);

public:
	ClickedTrigger();
	~ClickedTrigger();
	void setValues(json inValues);

	static rp3d::RigidBody* clicked;
	static rp3d::Vector3 worldPoint;
	static glm::vec3 direction;

	int force;

	void runSystemFunction(SystemBase* s);
	bool entityCheck(SystemBase* s, Entity* e);
};

#endif //BLOCKHOP_CLICKEDTRIGGER_H
