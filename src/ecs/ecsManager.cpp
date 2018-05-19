//
// Created by Jono on 03/03/2017.
//
#include "ecsManager.h"
#include "../logger.h"

ECSManager *ECSManager::c_instance = nullptr;

//Helpers for ECS
Entity* ECSManager::findEntity(const std::string &name) {
	auto it = gameEntities.find(name);
	if(it != gameEntities.end())
		return it->second;
	return nullptr;
}

//Generation of ESC Classes
Component* ECSManager::createComponent(const std::string &name, const json &compData) {
	try {
		//Get component from map and create a new instance
		auto createFunc = gameComponentExports.at(name);
		Component *t = createFunc();
		t->setName(name);
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

System* ECSManager::createSystem(const std::string &name, const std::vector<std::string> &compsNeeded, const json &inValues){
	try {
		//Get system from map and instantiate with a list of required components
		auto createFunc = gameSystemExports.at(name);
		System *t = createFunc();
		//Add to list of required components for this system
		t->setRequiredComponents(compsNeeded);
		try {
			t->setValues(inValues);
		} catch(...) {
			Logger()<< "Incorrect json object given to " << name << " @ "<<t;
		}
		//Add to global gameSystems store
		gameSystems.emplace_back(std::make_pair(name, t));
		return t;
	}
	catch (...) {
		Logger()<< "System type " << name << " doesn't exist.";
	}
	return nullptr;
};

Entity* ECSManager::createEntity(const std::string &name, const std::vector<std::string> &compsToSub, const std::vector<json> &compsData){
	/* Entity creation handles:
	 * - Creation of all components needed
	 * - Addition of components to entities and by extention, subscription to systems
	 */
	assert(compsToSub.size() == compsData.size());

	Entity* e = new Entity(name);
	//Get components and create each of them
	for(int i = 0 ; i < compsToSub.size(); i++) {
		Component* newComp = createComponent(compsToSub[i], compsData[i]);
		e->addComponent(newComp);
	}
	//Add to global gameEntities store
	gameEntities.insert(std::make_pair(name, e));
	return e;
};