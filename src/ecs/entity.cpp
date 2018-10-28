#include "entity.h"
#include "ecsManager.h"

Entity::Entity(std::string inName)
{
	name = inName;
}
Entity::~Entity()
{
	Logger() << "\tComponents:";
	for(auto component : subbedComponents)
	{
		Logger() << "\t\tDeleting " << component->getName() << " from " << getName();
		delete component;
	}
	Logger() << "\tTriggers:";
	for(auto trigger : subbedTriggers)
	{
		Logger() << "\t\tDeleting " << trigger->getName() << " from " << getName();
		delete trigger;
	}
}

void Entity::addComponent(ComponentBase* comp) {
	subbedComponents.push_back(comp);
	//After updating our subscribed components, redo system subscription
	subscribeToSystems();
	subscribeToActions();
}

void Entity::removeComponent(std::string compName) {
	auto it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [&compName](ComponentBase*& o) {
		return (o->getName() == compName);
	});

	if (it != subbedComponents.end())
	{
		subbedComponents.erase(it);
		delete *it;
		//After updating our subscribed components, do system unsubscription
		unsubscribeFromSystems();
		unsubscribeToActions();
	}
}

void Entity::addTrigger(TriggerBase *trig) {
	//TODO type safety surrounding trigger to system interaction

	subbedTriggers.push_back(trig);
	trig->subscribeEntityToActions(this);
}

void Entity::removeTrigger(std::string trigName) {
	auto it = std::find_if(subbedTriggers.begin(), subbedTriggers.end(), [&trigName](TriggerBase*& o) {
		return (o->getName() == trigName);
	});

	if (it != subbedTriggers.end())
	{
		subbedTriggers.erase(it);
		delete *it;
	}
}

std::vector<std::string> Entity::getComponents() const {
	std::vector<std::string> compNames;
	for(auto &comp : subbedComponents) {
		compNames.push_back(comp->getName());
	}
	return compNames;
}

void Entity::subscribeToActions() {
	//Get each attached triggers
	for(auto &trig : subbedTriggers) {

		//Pass entity to trigger to let it figure out which
		// actions it meets requirements for
		trig->subscribeEntityToActions(this);
	}
}

void Entity::unsubscribeToActions() {
	//Get each attached triggers
	for(auto &trig : subbedTriggers) {
		//Pass entity to trigger to let it figure out which
		// actions it meets requirements for
		trig->unsubscribeEntityFromActions(this);
	}
}

void Entity::subscribeToSystems() {
	//Move through each system
	for(auto &sys : ECSManager::get().gameSystems) {
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
	for(auto &sys : ECSManager::get().gameSystems) {
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

bool Entity::isType(std::string identifier) {
	return ECSManager::get().isType(this, identifier);
}

bool Entity::isExactType(std::string identifier) {
	return ECSManager::get().isExactType(this, identifier);
}
