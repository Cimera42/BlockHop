#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <vector>
#include <string>
#include "component.h"
#include "trigger.h"

class Entity {
public:
	/*
	 * A entity holds references to components which in turn allow
	 * it to be subscribed to specific systems. It is basically the
	 * connecting block which we worry about when creating a game.
	 * An entity does not need to exported as there are no derived
	 * types, instead they are created through the ECSManager.
	 */
	Entity(std::string inName);
	~Entity();

	/*
	 * Functions for adding/removing components from entities.
	 * Should be used to enable/disable functionality of an
	 * entity.
	 */
	void addComponent(ComponentBase* comp);
	void removeComponent(std::string compName);

	void addTrigger(TriggerBase* trig);
	void removeTrigger(std::string trigName);

	/*
	 * Simple getter for entity name
	 */
	std::string getName() const;

	bool isType(std::string identifier);
	bool isExactType(std::string identifier);

	/*
	 * Helpers for retrieving components attached to an entity
	 */
	std::vector<std::string> getComponents() const;
	template <typename T>
	T *getComponent()
	{
		auto it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [](ComponentBase*& o) {
			return (o->getName() == Component<T>::name);
		});

		if (it != subbedComponents.end())
			return static_cast<T*>(*it);
		return nullptr;
	}

	std::vector<TriggerBase*> getTriggers() {
		return subbedTriggers;
	}

	template <typename T>
	T* getTrigger() {
		auto it = std::find_if(subbedTriggers.begin(), subbedTriggers.end(), [](TriggerBase*& o) {
			return (o->getName() == Trigger<T>::name);
		});

		if (it != subbedTriggers.end())
			return static_cast<T*>(*it);
		return nullptr;
	}
//private:
	/*
	 * Functions for adding/removing triggers from entities.
	 * Called upon attaching a component to a system
	 */

	void subscribeToActions();
	void unsubscribeToActions();

	void subscribeToSystems();
	void unsubscribeFromSystems();
	std::vector<ComponentBase*> subbedComponents;
	std::vector<TriggerBase*> subbedTriggers;

	std::string name;
};

#endif // ENTITY_H_INCLUDED