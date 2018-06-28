//
// Created by Jono on 28/06/2018.
//

#include "trigger.h"

Trigger::Trigger() {}
Trigger::~Trigger() {}

bool Trigger::isInTriggerList(std::vector<Trigger*> triggerList) {
	std::string ourTrigName = getName();
	auto it = std::find_if(triggerList.begin(), triggerList.end(), [ourTrigName](Trigger*& t) {
		return t->getName() == ourTrigName;
	});

	return it != triggerList.end();
}