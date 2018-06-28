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
	void addComponent(Component* comp);
	void removeComponent(std::string compName);

	void addTrigger(Trigger* trig);
	void removeTrigger(std::string trigName);

	/*
	 * Simple getter for entity name
	 */
	std::string getName() const;

	/*
	 * Helpers for retrieving components attached to an entity
	 */
	std::vector<std::string> getComponents() const;
	template <typename T>
	T* getComponent(std::string compName) {
		auto it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [&compName](Component*& o) {
			return (o->getName() == compName);
		});

		if (it != subbedComponents.end())
			return static_cast<T*>(*it);
		return nullptr;
	}

	std::vector<Trigger*> getTriggers() const {
		return subbedTriggers;
	}
private:
	/*
	 * Functions for adding/removing triggers from entities.
	 * Called upon attaching a component to a system
	 */
	//void subscribeTriggersToEntity(System* subbedSys);
	//void unsubscribeTriggersToEntity(System* subbedSys);

	void subscribeToSystems();
	void unsubscribeFromSystems();
	std::vector<Component*> subbedComponents;
	std::vector<Trigger*> subbedTriggers;

	std::string name;
};

#endif // ENTITY_H_INCLUDED