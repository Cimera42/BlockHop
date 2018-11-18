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
#include <tuple>
#include "../logger.h"

typedef ComponentBase* (*ComponentFactoryPtr)();
typedef SystemBase* (*SystemFactoryPtr)();
typedef TriggerBase* (*TriggerFactoryPtr)();

#define COMPONENT_EXPORT(type, componentName) \
template<> const bool Component<type>::exported = ECSManager::get().exportComponent<type>(componentName); \
template<> const std::string Component<type>::name = componentName;
#define SYSTEM_EXPORT(type, systemName) \
template<> const bool System<type>::exported = ECSManager::get().exportSystem<type>(systemName); \
template<> const std::string System<type>::name = systemName;
#define TRIGGER_EXPORT(type, triggerName) \
template<> const bool Trigger<type>::exported = ECSManager::get().exportTrigger<type>(triggerName); \
template<> const std::string Trigger<type>::name = triggerName;

class ECSManager {
public:
	/* Singleton */
	static ECSManager& get()
	{
		static ECSManager c_instance;
		return c_instance;
	}
private:
	ECSManager();
	//Holds references to exports for use in creation
	std::map<std::string, SystemFactoryPtr> gameSystemExports;
	std::map<std::string, ComponentFactoryPtr> gameComponentExports;
	std::map<std::string, TriggerFactoryPtr> gameTriggerExports;

	// Systems have other behind the scenes logic for logic vs presentational - hence private
	int systemPresentationalIndex = 0;
	// Preserve ordering for systems: name, systempointer, isPresentational
	std::vector<std::tuple<std::string, SystemBase*, bool> > gameSystems;
	// TODO avoid storing systems twice
	// (however better than copying as per https://stackoverflow.com/questions/421573/best-way-to-extract-a-subvector-from-a-vector)
	std::vector<std::tuple<std::string, SystemBase*, bool> > presentationSystems;
	std::vector<std::tuple<std::string, SystemBase*, bool> > logicSystems;
public:
	ECSManager(ECSManager const&) = delete;
	void operator=(ECSManager const&) = delete;
	~ECSManager();
		/*
	 * Helper functions
	 * Will return nullptr if a match cannot be found
	 */
		template <typename T>
		T *findSystem()
	{
		auto it =
				std::find_if(gameSystems.begin(), gameSystems.end(), [](std::tuple<std::string, SystemBase*, bool> o) {
					return (std::get<0>(o) == System<T>::name);
				});
		if(it != gameSystems.end())
			return static_cast<T*>(std::get<1>(*it));
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
	SystemBase* createSystem(std::string name, bool isPresentational, std::vector<std::string> compsNeeded, std::vector<std::string> attachedTriggers);
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

	std::vector<std::tuple<std::string, SystemBase*, bool> > getGameSystems();
	std::vector<std::tuple<std::string, SystemBase*, bool> > getPresentationSystems();
	std::vector<std::tuple<std::string, SystemBase*, bool> > getLogicSystems();

	//Used by systems to retrieve instances do not use otherwise
	std::map<std::string, TriggerBase*> gameTriggers;
};

#endif //BLOCKHOP_ECSMANAGER_H
