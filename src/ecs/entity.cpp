#include "entity.h"
#include "ecsManager.h"
#include "../logger.h"

Entity::Entity(std::string inName)
{
	name = inName;
}
Entity::~Entity(){}

void Entity::addComponent(Component* comp) {
	subbedComponents.push_back(comp);
	//After updating our subscribed components, redo system subscription
	subscribeToSystems();
}

void Entity::removeComponent(std::string compName) {
	auto it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [&compName](Component*& o) {
		return (o->getName() == compName);
	});

	if (it != subbedComponents.end())
		subbedComponents.erase(it);
	//After updating our subscribed components, do system unsubscription
	unsubscribeFromSystems();
}

std::vector<std::string> Entity::getComponents() const {
	std::vector<std::string> compNames;
	for(auto &comp : subbedComponents) {
		compNames.push_back(comp->getName());
	}
	return compNames;
}

void Entity::subscribeToSystems() {
	//Move through each system
	for(auto &sys : ECSManager::i()->gameSystems) {
		auto sysPtr = sys.second;

		//Attempt to subscribe.
		//A false return type means that either the entity is already subbed
		//or doesn't meet the requirements
		if(sysPtr->subscribeEntity(this)) {
			sysPtr->subscribeCallback(this);
			Logger() << "Entity \"" << this->getName() << "\" successfully subscribed to "<<sys.first;
		}
	}
}

void Entity::unsubscribeFromSystems() {
	//Move through each system
	for(auto &sys : ECSManager::i()->gameSystems) {
		auto sysPtr = sys.second;

		if(sysPtr->hasEntity(this) && !sysPtr->hasRequired(this))
		{
			//Attempt to unsubscribe.
			//A false return type means that the entity is not subbed
			if(sysPtr->unsubscribeEntity(this)) {
				sysPtr->unsubscribeCallback(this);
				Logger() << "Entity \"" << this->getName() << "\" successfully unsubscribed from "<<sys.first;
			}
		}
	}
}

std::string Entity::getName() const
{
	return name;
}
