//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_CLICKEDTRIGGER_H
#define BLOCKHOP_CLICKEDTRIGGER_H

#include "../ecs/trigger.h"
#include "../systems/PhysicsSystem.h"
#include <glm/glm.hpp>

class ClickedTrigger : public Trigger {
	static bool exported;

public:
	ClickedTrigger();
	~ClickedTrigger();
	void setValues(json inValues);

	static rp3d::RigidBody* clicked;
	static rp3d::Vector3 worldPoint;
	static glm::vec3 direction;

	void runSystemFunction(System* s);
	void runEntityCheck(System* s, Entity* e);
};

#endif //BLOCKHOP_CLICKEDTRIGGER_H
