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

typedef ComponentBase* (*ComponentFactoryPtr)();
typedef SystemBase* (*SystemFactoryPtr)();
typedef TriggerBase* (*TriggerFactoryPtr)();

#define COMPONENT_EXPORT(type, componentName) \
template<> const bool Component<type>::exported = ECSManager::i()->exportComponent<type>(componentName); \
template<> const std::string Component<type>::name = componentName;
#define SYSTEM_EXPORT(type, systemName) \
template<> const bool System<type>::exported = ECSManager::i()->exportSystem<type>(systemName); \
template<> const std::string System<type>::name = systemName;
#define TRIGGER_EXPORT(type, triggerName) \
template<> const bool Trigger<type>::exported = ECSManager::i()->exportTrigger<type>(triggerName); \
template<> const std::string Trigger<type>::name = triggerName;

class ECSManager {
public:
	ECSManager();
	~ECSManager();

	/*
	 * Helper functions
	 * Will return nullptr if a match cannot be found
	 */
	template <typename T>
	T* findSystem() {
		auto it =
				std::find_if(gameSystems.begin(), gameSystems.end(), [](std::pair<std::string, SystemBase*> o) {
					return (o.first == System<T>::name);
				});
		if(it != gameSystems.end())
			return static_cast<T*>(it->second);
		return nullptr;
	}
	Entity* findEntity(std::string name);
	bool isExactType(Entity* e, std::string identifier);
	bool isType(Entity* e, std::string identifier);

	/*
	 * Functions for the generation of game elements
	 */
	ComponentBase* createComponent(std::string name, json compData);
	TriggerBase* createTrigger(std::string name, json trigData);
	SystemBase* createSystem(std::string name, std::vector<std::string> compsNeeded, std::vector<std::string> attachedTriggers);
	Entity* createEntity(std::string name, std::vector<std::string> compsToSub, std::vector<json> compsData, std::vector<std::string> trigsToSub, std::vector<json> trigsData);

	//Used to generate references to systems by string
	template<typename T>
	bool exportSystem(std::string name) {
		SystemFactoryPtr sysFunc = &SystemBase::create<T>;
		gameSystemExports.insert(std::make_pair(name, sysFunc));
		return true;
	}

	//Used to generate references to components, allowing for creation by string
	template<typename T>
	bool exportComponent(std::string name) {
		ComponentFactoryPtr compFunc = &ComponentBase::create<T>;
		gameComponentExports.insert(std::make_pair(name, compFunc));
		return true;
	}

	//Used to generate references to trigger, allowing for creation by string
	//Also generate a
	template<typename T>
	bool exportTrigger(std::string name) {
		TriggerFactoryPtr trigFunc = &TriggerBase::create<T>;
		gameTriggerExports.insert(std::make_pair(name, trigFunc));

		TriggerBase *t = trigFunc();
		gameTriggers.insert(std::make_pair(name, t));
		return true;
	}

	/*
	 * ECS data accessible to public
	 * TODO Add getters/settings and force this to be private
	 */
	std::map<std::string, std::vector<std::string> > gameIdentities;
	std::map<std::string, Entity*> gameEntities;
	//std::map<std::string, SystemBase*> gameSystems;
	std::vector<std::pair<std::string, SystemBase*> > gameSystems;

	//Used by systems to retrieve instances do not use otherwise
	std::map<std::string, TriggerBase*> gameTriggers;

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
