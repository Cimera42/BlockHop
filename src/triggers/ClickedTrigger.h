//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_CLICKEDTRIGGER_H
#define BLOCKHOP_CLICKEDTRIGGER_H

#include "../ecs/trigger.h"
#include "../systems/PhysicsSystem.h"
#include <glm/glm.hpp>

struct HitData
{
	btVector3 direction;
	btVector3 localPoint;

//	friend std::ostream &operator<< (std::ostream &os, HitData const &c) {
//		os << c.direction << " - " << c.localPoint;
//		return os;
//	}
};

class ClickedTrigger : public Trigger<ClickedTrigger>
{
public:
	ClickedTrigger();
	~ClickedTrigger();
	void setValues(json inValues);

	int force;

	static std::map<Entity*, HitData> hitEntities;
	static int indicatorAccumulator;

	void runSystemFunction(SystemBase* s);
	bool entityCheck(SystemBase* s, Entity* e);

	void doForce(SystemBase* s, Entity* e);
};

#endif //BLOCKHOP_CLICKEDTRIGGER_H
