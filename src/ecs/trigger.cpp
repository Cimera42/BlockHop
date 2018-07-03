//
// Created by Jono on 28/06/2018.
//

#include "trigger.h"

Trigger::Trigger() {}
Trigger::~Trigger() {}

void Trigger::addAction(Action *act) {
	subbedActions.push_back(act);
}

void Trigger::removeAction(std::string actName) {
	auto it = std::find_if(subbedActions.begin(), subbedActions.end(), [&actName](Action*& a) {
		return (a->getName() == actName);
	});

	if (it != subbedActions.end()) {
		subbedActions.erase(it);
	}
}

void Trigger::runActions(System *s, Entity *e) {
	for(auto act : subbedActions) {
		act->runAction(s, e, this);
	}
}

bool Trigger::isInTriggerList(std::vector<Trigger*> triggerList) {
	std::string ourTrigName = getName();
	auto it = std::find_if(triggerList.begin(), triggerList.end(), [ourTrigName](Trigger*& t) {
		return t->getName() == ourTrigName;
	});

	return it != triggerList.end();
}