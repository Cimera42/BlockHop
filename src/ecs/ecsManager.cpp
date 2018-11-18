//
// Created by Jono on 03/03/2017.
//
#include "ecsManager.h"

ECSManager::ECSManager() {}
ECSManager::~ECSManager()
{
	for(auto entity : gameEntities)
	{
		Logger() << "Deleting entity " << entity.second->getName();
		delete entity.second;
	}
	for(auto system : gameSystems)
	{
		Logger() << "Deleting system " << std::get<1>(system)->getName();
		delete std::get<1>(system);
	}
	for(auto trigger : gameTriggers)
	{
		Logger() << "Deleting trigger " << trigger.second->getName();
		delete trigger.second;
	}
}

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
ComponentBase* ECSManager::createComponent(std::string name, json compData) {
	try {
		//Get component from map and create a new instance
		auto createFunc = gameComponentExports.at(name);
		ComponentBase *t = createFunc();
		try {
			t->setValues(compData);
		} catch(...) {
			Logger()<< "Incorrect json object given to " << name << " @ "<<t;
		}
		return t;
	}
	catch (...) {
		Logger()<< "Component type " << name << " doesn't exist.";
	}
	return nullptr;
}

TriggerBase* ECSManager::createTrigger(std::string name, json trigData) {
	try {
		//Get trigger from map and create a new instance
		auto createFunc = gameTriggerExports.at(name);
		TriggerBase *t = createFunc();

		//Get the system this trigger is to be attached to
		bool foundSystem = false;
		for(std::tuple<std::string, SystemBase*, bool> o : gameSystems) {
			std::vector<TriggerBase*> triggers = std::get<1>(o)->getTriggers();
			auto it = std::find_if(triggers.begin(), triggers.end(), [name](TriggerBase *t) {
				return t->getName() == name;
			});
			if (it != triggers.end()) {
				//Set name in trigger
				t->setSystemName(std::get<0>(o));
				foundSystem = true;
				break;
			}
		}
		if(!foundSystem) {
			throw("No system with this trigger");
		}

		try {
			//Set trigger values
			t->setValues(trigData);
		} catch(...) {
			Logger()<< "Incorrect json object given to " << name << " @ "<<t;
		}
		return t;
	}
	catch (...) {
		Logger()<< "Trigger type " << name << " doesn't exist.";
	}
	return nullptr;
}

SystemBase* ECSManager::createSystem(std::string name, bool isPresentational, std::vector<std::string> compsNeeded, std::vector<std::string> attachedTriggers){
	try {
		//Get system from map and instantiate with a list of required components
		auto createFunc = gameSystemExports.at(name);
		SystemBase *t = createFunc();
		//Add to list of required components for this system
		t->setRequiredComponents(compsNeeded);
		//Add triggers that will be attached to this system
		t->setAttachedTriggers(attachedTriggers);
		//Add to global gameSystems store
		auto sys = std::make_tuple(name, t, isPresentational);
		gameSystems.push_back(sys);
		//Also add to respective system
		if(isPresentational) {
			presentationSystems.push_back(sys);
		} else {
			logicSystems.push_back(sys);
		}
		return t;
	}
	catch (...) {
		Logger()<< "System type " << name << " doesn't exist.";
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
		ComponentBase* newComp = createComponent(compsToSub[i], compsData[i]);
		if(newComp) {
			e->addComponent(newComp); //TODO wrap in try catch and delete if failure
		}
	}

	for(int i = 0; i < trigsToSub.size(); i++) {
		TriggerBase* newTrig = createTrigger(trigsToSub[i], trigsData[i]);
		//TODO check that trigger fufils requirements of system - ie. has correct components
		if(newTrig) {
			e->addTrigger(newTrig); // TODO wrap in try catch and delete if failure
		}
	}

	//Add to global gameEntities store
	gameEntities.insert(std::make_pair(name, e));
	return e;
}

std::vector<std::tuple<std::string, SystemBase *, bool> > ECSManager::getGameSystems() {
	return this->gameSystems;
}

std::vector<std::tuple<std::string, SystemBase *, bool> > ECSManager::getPresentationSystems() {
	return this->presentationSystems;
}

std::vector<std::tuple<std::string, SystemBase *, bool> > ECSManager::getLogicSystems() {
	return this->logicSystems;
};