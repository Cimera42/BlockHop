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

enum actionMode {
	EXACT_ONLY, //isExactType and will reject remaining
	EXACT, //isExactType but will accept reminaing
	INEXACT_ONLY, //isType and will reject remaining
	INEXACT, //isType but will accept remaining
};

class Trigger {
protected:
	typedef void (Trigger::*RunTrigFunc)(System* s, Entity* e);
	// Essentially a callback function that's attached on a per trigger basis that
	// will run for a specific identity type.
	void addAction(std::string identifier, actionMode mode, RunTrigFunc func);

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

	virtual const std::string getName() = 0;

	void setSystemName(std::string inSystemName) {systemName = inSystemName; }
	std::string getSystemName() {return systemName; };

	bool isInTriggerList(std::vector<Trigger*> triggerList);

	void subscribeEntityToActions(Entity* ent);
	void unsubscribeEntityFromActions(Entity* ent);

	//System level - called once per system update
	virtual void runSystemFunction(System* a) {};

	//Entity level - called for every entity subscribed to system
	void runEntityCheck(System* s, Entity* e);
	virtual bool entityCheck(System* s, Entity* e) { return false; };

private:
	std::string systemName;
	//List of function pointers inside Triggers -
	std::vector<std::tuple<std::string, actionMode, RunTrigFunc> > registeredActions;
	//TODO dont even think we need the identity info?
	std::vector<std::tuple<std::string, actionMode, RunTrigFunc> > subbedActions;
};

template <typename T>
class TriggerStatics : public Trigger
{
public:
	static const std::string name;
	const std::string getName() { return name; }

	/*
	 * Used to export so that ECSManager can see a specific component
	 * Usage inside components's cpp file:
	 *	  bool Component::exported = ECSManager::exportComponent<ComponentClass>("componentName");
	 */
	static const bool exported;
};

#endif //BLOCKHOP_TRIGGER_H
