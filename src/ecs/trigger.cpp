//
// Created by Jono on 28/06/2018.
//

#include "trigger.h"

Trigger::Trigger() {}
Trigger::~Trigger() {}

void Trigger::runEntityCheck(System *s, Entity *e) {
	//Run override
	entityCheck(s,e);
	/*if(entityCheck(s,e)) {
		//Possibly do something here - callback etc
	}*/
}

bool Trigger::isInTriggerList(std::vector<Trigger*> triggerList) {
	std::string ourTrigName = getName();
	auto it = std::find_if(triggerList.begin(), triggerList.end(), [ourTrigName](Trigger*& t) {
		return t->getName() == ourTrigName;
	});

	return it != triggerList.end();
}