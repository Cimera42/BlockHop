//
// Created by Jono on 03/03/2017.
//
#include "ecsManager.h"

ECSManager *ECSManager::c_instance = 0;

ECSManager::ECSManager() {}
ECSManager::~ECSManager() {}

//Helpers for ECS
Entity* ECSManager::findEntity(std::string name) {
	auto it = gameEntities.find(name);
	if(it != gameEntities.end())
		return it->second;
	return nullptr;
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
			Logger()<< "Incorrect json object given to " << name << " @ "<<t;
		}
		return t;
	}
	catch (...) {
		Logger()<< "Component type " << name << " doesn't exist.";
	}
	return nullptr;
}

System* ECSManager::createSystem(std::string name, std::vector<std::string> compsNeeded){
	try {
		//Get system from map and instantiate with a list of required components
		auto createFunc = gameSystemExports.at(name);
		System *t = createFunc();
		//Add to list of required components for this system
		t->setRequiredComponents(compsNeeded);
		//Add to global gameSystems store
		gameSystems.push_back(std::make_pair(name, t));
		return t;
	}
	catch (...) {
		Logger()<< "System type " << name << " doesn't exist.";
	}
	return nullptr;
};

Entity* ECSManager::createEntity(std::string name, std::vector<std::string> compsToSub, std::vector<json> compsData){
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