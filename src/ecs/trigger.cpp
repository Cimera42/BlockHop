//
// Created by Jono on 28/06/2018.
//

#include "trigger.h"
#include "entity.h"
#include "ecsManager.h"

TriggerBase::TriggerBase() {}
TriggerBase::~TriggerBase() {}

void TriggerBase::addAction(std::string identifier, actionMode mode, RunTrigFunc func) {
	registeredActions.push_back(std::make_tuple(identifier, mode, func));
}

void TriggerBase::subscribeEntityToActions(Entity *ent) {
	auto compNames = ent->getComponents();

	//Move through registered trigger actions and sub to valid ones
	for(auto action : registeredActions) {
		std::string identity = std::get<0>(action);
		actionMode mode = std::get<1>(action);
		//In mode, check correct type and attempt to subscribe
		if(mode == EXACT_ONLY) {
			if(ECSManager::i()->isExactType(ent, identity)) {
				//Subscribe and break
				subbedActions.push_back(action);
				break;
			}
		} else if(mode == EXACT) {
			if(ECSManager::i()->isExactType(ent, identity)) {
				//Subscribe but dont break
				subbedActions.push_back(action);
			}
		} else if(mode == INEXACT_ONLY) {
			if(ECSManager::i()->isType(ent, identity)) {
				//Subscribe and break
				subbedActions.push_back(action);
				break;
			}
		} else if(mode == INEXACT) {
			if(ECSManager::i()->isType(ent, identity)) {
				//Subscribe but dont break
				subbedActions.push_back(action);
			}
		}
	}
}

void TriggerBase::unsubscribeEntityFromActions(Entity *ent) {
	//Easiest way, erase vector and redo from ground up (avoid complications)
	//TODO efficiency if a lot of component switching on the entity!
	subbedActions.clear();
	subscribeEntityToActions(ent);
}

void TriggerBase::runEntityCheck(SystemBase *s, Entity *e) {
	//Run override
	if(entityCheck(s,e)) {
		//Run trigger actions
		for(auto action : subbedActions) {
			RunTrigFunc func = std::get<2>(action);
			(this->*func)(s,e);
		}
	};
}

bool TriggerBase::isInTriggerList(std::vector<TriggerBase*> triggerList) {
	std::string ourTrigName = getName();
	auto it = std::find_if(triggerList.begin(), triggerList.end(), [ourTrigName](TriggerBase*& t) {
		return t->getName() == ourTrigName;
	});

	return it != triggerList.end();
}