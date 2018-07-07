//
// Created by Jono on 03/03/2017.
//
#include "ecsManager.h"

ECSManager *ECSManager::c_instance = 0;

ECSManager::ECSManager() {}
ECSManager::~ECSManager() {}

//TODO upon failure of creation, need to actually unsubscribe/deallocate resources!

//Helpers for ECS
Entity* ECSManager::findEntity(std::string name) {
	auto it = gameEntities.find(name);
	if(it != gameEntities.end())
		return it->second;
	return nullptr;
}

bool ECSManager::isType(Entity * e, std::string identifier) {
	auto componentsNeeded = gameIdentities.find(identifier);
	if(componentsNeeded != gameIdentities.end()) {
		auto comps = e->getComponents();

		std::sort(comps.begin(), comps.end());
		std::sort(componentsNeeded->second.begin(), componentsNeeded->second.end());

		//Every in componentsNeeded is in comps
		if(std::includes(comps.begin(), comps.end(),
						 componentsNeeded->second.begin(), componentsNeeded->second.end())) {
			return true;
		}
	}
	return false;
}

bool ECSManager::isExactType(Entity *e, std::string identifier) {
	auto componentsNeeded = gameIdentities.find(identifier);
	if(componentsNeeded != gameIdentities.end()) {
		auto comps = e->getComponents();

		std::sort(comps.begin(), comps.end());
		std::sort(componentsNeeded->second.begin(), componentsNeeded->second.end());

		if(componentsNeeded->second == comps) {
			return true;
		}
	}
	return false;
}

//Generation of ESC Classes
Component* ECSManager::createComponent(std::string name, json compData) {
	try {
		//Get component from map and create a new instance
		auto createFunc = gameComponentExports.at(name);
		Component *t = createFunc();
		t->setName(name);
		try {
			t->setValues(compData);
		} catch(...) {
			Logger(1)<< "Incorrect json object given to " << name << " @ "<<t;
		}
		return t;
	}
	catch (...) {
		Logger(1)<< "Component type " << name << " doesn't exist.";
	}
	return nullptr;
}

Trigger* ECSManager::createTrigger(std::string name, json trigData) {
	try {
		//Get trigger from map and create a new instance
		auto createFunc = gameTriggerExports.at(name);
		Trigger *t = createFunc();
		t->setName(name);

		//Get the system this trigger is to be attached to
		bool foundSystem = false;
		for(std::pair<std::string, System*> o : gameSystems) {
			std::vector<Trigger*> triggers = o.second->getTriggers();
			auto it = std::find_if(triggers.begin(), triggers.end(), [name](Trigger *t) {
				return t->getName() == name;
			});
			if (it != triggers.end()) {
				//Set name in trigger
				t->setSystemName(o.first);
				foundSystem = true;
			}
		}
		if(!foundSystem) {
			throw("No system with this trigger");
		}

		try {
			//Set trigger values
			t->setValues(trigData);
		} catch(...) {
			Logger(1)<< "Incorrect json object given to " << name << " @ "<<t;
		}
		return t;
	}
	catch (...) {
		Logger(1)<< "Trigger type " << name << " doesn't exist.";
	}
	return nullptr;
}

System* ECSManager::createSystem(std::string name, std::vector<std::string> compsNeeded, std::vector<std::string> attachedTriggers){
	try {
		//Get system from map and instantiate with a list of required components
		auto createFunc = gameSystemExports.at(name);
		System *t = createFunc();
		//Add to list of required components for this system
		t->setRequiredComponents(compsNeeded);
		//Add triggers that will be attached to this system
		t->setAttachedTriggers(attachedTriggers);
		//Add to global gameSystems store
		gameSystems.push_back(std::make_pair(name, t));
		return t;
	}
	catch (...) {
		Logger(1)<< "System type " << name << " doesn't exist.";
	}
	return nullptr;
};

Entity* ECSManager::createEntity(std::string name, std::vector<std::string> compsToSub, std::vector<json> compsData, std::vector<std::string> trigsToSub, std::vector<json> trigsData){
	/* Entity creation handles:
	 * - Creation of all components needed
	 * - Addition of components to entities and by extention, subscription to systems
	 * - By extension then also triggers attached to entities
	 */
	assert(compsToSub.size() == compsData.size());

	Entity* e = new Entity(name);
	//Get components and create each of them
	for(int i = 0; i < compsToSub.size(); i++) {
		Component* newComp = createComponent(compsToSub[i], compsData[i]);
		if(newComp) {
			e->addComponent(newComp);
		}
	}

	for(int i = 0; i < trigsToSub.size(); i++) {
		Trigger* newTrig = createTrigger(trigsToSub[i], trigsData[i]);
		//TODO check that trigger fufils requirements of system (and actions) - ie. has correct components
		if(newTrig) { //TODO remove this check - its just if creation fails - do other TODOs instead
			e->addTrigger(newTrig); // TODO wrap in try catch and delete newTrig if failure
		}
	}

	//Add to global gameEntities store
	gameEntities.insert(std::make_pair(name, e));
	return e;
};