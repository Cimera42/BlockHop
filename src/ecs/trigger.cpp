//
// Created by Jono on 28/06/2018.
//

#include "trigger.h"
#include "entity.h"
#include "ecsManager.h"

Trigger::Trigger() {}
Trigger::~Trigger() {}

void Trigger::addTriggerRunner(std::string identifier, runnerMode mode, RunTrigFunc func) {
	registeredTriggerRunners.push_back(std::make_tuple(identifier, mode, func));
}

void Trigger::subscribeEntityToRunners(Entity *ent) {
	auto compNames = ent->getComponents();

	//Move through registered trigger runners and sub to valid ones
	for(auto runner : registeredTriggerRunners) {
		std::string identity = std::get<0>(runner);
		runnerMode mode = std::get<1>(runner);
		//In mode, check correct type and attempt to subscribe
		if(mode == EXACT_ONLY) {
			if(ECSManager::i()->isExactType(ent, identity)) {
				//Subscribe and break
				subbedTriggerRunners.push_back(runner);
				break;
			}
		} else if(mode == EXACT) {
			if(ECSManager::i()->isExactType(ent, identity)) {
				//Subscribe but dont break
				subbedTriggerRunners.push_back(runner);
			}
		} else if(mode == INEXACT_ONLY) {
			if(ECSManager::i()->isType(ent, identity)) {
				//Subscribe and break
				subbedTriggerRunners.push_back(runner);
				break;
			}
		} else if(mode == INEXACT) {
			if(ECSManager::i()->isType(ent, identity)) {
				//Subscribe but dont break
				subbedTriggerRunners.push_back(runner);
			}
		}
	}
}

void Trigger::unsubscribeEntityFromRunners(Entity *ent) {
	//Easiest way, erase vector and redo from ground up (avoid complications)
	//TODO efficiency if a lot of component switching on the entity!
	subbedTriggerRunners.clear();
	subscribeEntityToRunners(ent);
}

void Trigger::runEntityCheck(System *s, Entity *e) {
	//Run override
	if(entityCheck(s,e)) {
		//Run trigger runners
		for(auto runner : subbedTriggerRunners) {
			RunTrigFunc func = std::get<2>(runner);
			(this->*func)(s,e);
		}
	};
}

bool Trigger::isInTriggerList(std::vector<Trigger*> triggerList) {
	std::string ourTrigName = getName();
	auto it = std::find_if(triggerList.begin(), triggerList.end(), [ourTrigName](Trigger*& t) {
		return t->getName() == ourTrigName;
	});

	return it != triggerList.end();
}