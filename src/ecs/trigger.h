//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_TRIGGER_H
#define BLOCKHOP_TRIGGER_H

#include <vector>
#include <json.hpp>
class System;
class Entity;

using json = nlohmann::json;

class Trigger {
public:
	/*
	 * A trigger is an entity level object that completes checks in a specific system
	 * and causes flow on action effects. A trigger is first exported via the ECSManager
	 * and then created through the ECSManager.
	 */
	Trigger();
	virtual ~Trigger();

	/*
	 * create() is used during the creation process of the trigger
	 * by the ECSManager.
	 * setValues() takes in a json object and sets the initial data
	 * state for the trigger. This is virtual and must be implemented
	 * by each different trigger seperately!
	 * In addition, setValues should implement checks to test that
	 * the json object contains valid data. *Else an exception
	 * should be thrown.* TODO Verify the behaviour we want
	 * Do not use either of these outside of ECSManager!
	 */
	template<typename T>
	static Trigger* create() {return new T; };
	virtual void setValues(json inValues) = 0;
	void setName(std::string inName) {name = inName; };
	std::string getName() {return name; };
	void setSystemName(std::string inSystemName) {systemName = inSystemName; }
	std::string getSystemName() {return systemName; };

	//System level
	virtual void runSystemFunction(System* a) {};

	//Entity level
	virtual void runEntityCheck(System* s, Entity* e) {}; //TODO figure out a way to extend this to add in more data if needed (ie not repeating things between triggers)
	bool isInTriggerList(std::vector<Trigger*> triggerList);

private:
	/*
	 * Used to export so that ECSManager can see a specific trigger
	 * Usage inside triggers's cpp file:
	 *	  bool Trigger::exported = ECSManager::exportComponent<ComponentClass>("componentName");
	 */
	//static bool exported;

	std::string name;
	std::string systemName;
};

#endif //BLOCKHOP_TRIGGER_H
