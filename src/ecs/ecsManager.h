//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_ECSMANAGER_H
#define BLOCKHOP_ECSMANAGER_H

#include "entity.h"
#include "component.h"
#include "system.h"
#include <string>
#include <map>
#include <vector>

typedef Component* (*ComponentFactoryPtr)();
typedef System* (*SystemFactoryPtr)();

#define COMPONENT_EXPORT(type, name) bool type::exported = ECSManager::i()->exportComponent<type>(name);//#type to use <X>Component as a string literal
#define SYSTEM_EXPORT(type, name) bool type::exported = ECSManager::i()->exportSystem<type>(name);

class ECSManager {
public:
	ECSManager() = default;
	~ECSManager() = default;

	/*
	 * Helper functions
	 * Will return nullptr if a match cannot be found
	 */
	template <typename T>
	T* findSystem(const std::string &name) {
		auto it =
				std::find_if(gameSystems.begin(), gameSystems.end(), [&name](std::pair<std::string, System*> o) {
					return (o.first == name);
				});
		if(it != gameSystems.end())
			return static_cast<T*>(it->second);
		return nullptr;
	}
	Entity* findEntity(const std::string &name);

	/*
	 * Functions for the generation of game elements
	 */
	Component* createComponent(const std::string &name, const json &compData);
	System* createSystem(const std::string &name, const std::vector<std::string> &compsNeeded, const json &inValues);
	Entity* createEntity(const std::string &name, const std::vector<std::string> &compsToSub, const std::vector<json> &compsData);

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

	/*
	 * ECS data accessible to public
	 * TODO Add getters/settings and force this to be private
	 */
	std::map<std::string, Entity*> gameEntities;
	//std::map<std::string, System*> gameSystems;
	std::vector<std::pair<std::string, System*> > gameSystems;

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
};

#endif //BLOCKHOP_ECSMANAGER_H
