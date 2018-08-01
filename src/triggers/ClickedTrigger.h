//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_CLICKEDTRIGGER_H
#define BLOCKHOP_CLICKEDTRIGGER_H

#include "../ecs/trigger.h"
#include "../systems/PhysicsSystem.h"
#include <glm/glm.hpp>

class ClickedTrigger : public Trigger<ClickedTrigger>
{
public:
	ClickedTrigger();
	~ClickedTrigger();
	void setValues(json inValues);

	int force;

	void runSystemFunction(SystemBase* s);
	bool entityCheck(SystemBase* s, Entity* e);
};

#endif //BLOCKHOP_CLICKEDTRIGGER_H
