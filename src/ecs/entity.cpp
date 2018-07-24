#include "entity.h"
#include "ecsManager.h"

Entity::Entity(std::string inName)
{
	name = inName;
}
Entity::~Entity(){}

void Entity::addComponent(Component* comp) {
	subbedComponents.push_back(comp);
	//After updating our subscribed components, redo system subscription
	subscribeToSystems();
	subscribeToTriggerRunners();
}

void Entity::removeComponent(std::string compName) {
	auto it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [&compName](Component*& o) {
		return (o->getName() == compName);
	});

	if (it != subbedComponents.end())
		subbedComponents.erase(it);
	//After updating our subscribed components, do system unsubscription
	unsubscribeFromSystems();
	unsubscribeToTriggerRunners();
}

void Entity::addTrigger(Trigger *trig) {
	//TODO type safety surrounding trigger to system interaction

	subbedTriggers.push_back(trig);
	trig->subscribeEntityToRunners(this);
}

void Entity::removeTrigger(std::string trigName) {
	auto it = std::find_if(subbedTriggers.begin(), subbedTriggers.end(), [&trigName](Trigger*& o) {
		return (o->getName() == trigName);
	});

	if (it != subbedTriggers.end())
		subbedTriggers.erase(it);
}

std::vector<std::string> Entity::getComponents() const {
	std::vector<std::string> compNames;
	for(auto &comp : subbedComponents) {
		compNames.push_back(comp->getName());
	}
	return compNames;
}

void Entity::subscribeToTriggerRunners() {
	//Get each attached triggers
	for(auto &trig : subbedTriggers) {

		//Pass entity to trigger to let it figure out which
		// runners it meets requirements for
		trig->subscribeEntityToRunners(this);
	}
}

void Entity::unsubscribeToTriggerRunners() {
	//Get each attached triggers
	for(auto &trig : subbedTriggers) {
		//Pass entity to trigger to let it figure out which
		// runners it meets requirements for
		trig->unsubscribeEntityFromRunners(this);
	}
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
			Logger(1) << "Entity \"" << this->getName() << "\" successfully subscribed to "<<sys.first;
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
				Logger(1) << "Entity \"" << this->getName() << "\" successfully unsubscribed from "<<sys.first;
			}
		}
	}
}

std::string Entity::getName() const
{
	return name;
}

bool Entity::isType(std::string identifier) {
	return ECSManager::i()->isType(this, identifier);
}

bool Entity::isExactType(std::string identifier) {
	return ECSManager::i()->isExactType(this, identifier);
}
