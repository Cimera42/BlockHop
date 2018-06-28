//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_ECSMANAGER_H
#define BLOCKHOP_ECSMANAGER_H

#include "entity.h"
#include "component.h"
#include "system.h"
#include "trigger.h"
#include <string>
#include <map>
#include <vector>
#include "../logger.h"

typedef Component* (*ComponentFactoryPtr)();
typedef System* (*SystemFactoryPtr)();
typedef Trigger* (*TriggerFactoryPtr)();

#define COMPONENT_EXPORT(type, name) bool type::exported = ECSManager::i()->exportComponent<type>(name);//#type to use <X>Component as a string literal
#define SYSTEM_EXPORT(type, name) bool type::exported = ECSManager::i()->exportSystem<type>(name);
#define TRIGGER_EXPORT(type, name) bool type::exported = ECSManager::i()->exportTrigger<type>(name);

class ECSManager {
public:
	ECSManager();
	~ECSManager();

	/*
	 * Helper functions
	 * Will return nullptr if a match cannot be found
	 */
	template <typename T>
	T* findSystem(std::string name) {
		auto it =
				std::find_if(gameSystems.begin(), gameSystems.end(), [&name](std::pair<std::string, System*> o) {
					return (o.first == name);
				});
		if(it != gameSystems.end())
			return static_cast<T*>(it->second);
		return nullptr;
	}
	Entity* findEntity(std::string name);

	/*
	 * Functions for the generation of game elements
	 */
	Component* createComponent(std::string name, json compData);
	Trigger* createTrigger(std::string name, json trigData);
	System* createSystem(std::string name, std::vector<std::string> compsNeeded, std::vector<std::string> attachedTriggers);
	Entity* createEntity(std::string name, std::vector<std::string> compsToSub, std::vector<json> compsData, std::vector<std::string> trigsToSub, std::vector<json> trigsData);

	//Used to generate references to systems by string
	template<typename T>
	bool exportSystem(std::string name) {
		SystemFactoryPtr sysFunc = &System::create<T>;
		gameSystemExports.insert(std::make_pair(name, sysFunc));
		return true;
	}

	//Used to generate references to components, allowing for creation by string
	template<typename T>
	bool exportComponent(std::string name) {
		ComponentFactoryPtr compFunc = &Component::create<T>;
		gameComponentExports.insert(std::make_pair(name, compFunc));
		return true;
	}

	//Used to generate references to trigger, allowing for creation by string
	//Also generate a
	template<typename T>
	bool exportTrigger(std::string name) {
		TriggerFactoryPtr trigFunc = &Trigger::create<T>;
		gameTriggerExports.insert(std::make_pair(name, trigFunc));

		Trigger *t = trigFunc();
		t->setName(name);
		gameTriggers.insert(std::make_pair(name, t));
		return true;
	}

	/*
	 * ECS data accessible to public
	 * TODO Add getters/settings and force this to be private
	 */
	std::map<std::string, Entity*> gameEntities;
	//std::map<std::string, System*> gameSystems;
	std::vector<std::pair<std::string, System*> > gameSystems;

	//Used by systems to retrieve instances do not use otherwise
	std::map<std::string, Trigger*> gameTriggers;

	/*
	 * Singleton pattern. Must use i()-> to access any class methods.
	 * Prevents static initialisation hell.
	 */
	static ECSManager *i()
	{
		if (!c_instance)
			c_instance = new ECSManager;
		return c_instance;
	}

private:
	static ECSManager *c_instance;
	//Holds references to exports for use in creation
	std::map<std::string, SystemFactoryPtr> gameSystemExports;
	std::map<std::string, ComponentFactoryPtr> gameComponentExports;
	std::map<std::string, TriggerFactoryPtr> gameTriggerExports;
};

#endif //BLOCKHOP_ECSMANAGER_H
