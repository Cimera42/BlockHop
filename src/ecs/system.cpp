#include "system.h"
#include "ecsManager.h"

SystemBase::SystemBase() {}
SystemBase::~SystemBase() {}

void SystemBase::setRequiredComponents(std::vector<std::string> inComps) {
	requiredComps = inComps;
}

std::vector<std::string> SystemBase::getRequiredComponents() {
	return requiredComps;
}

void SystemBase::setAttachedTriggers(std::vector<std::string> inTrigs) {
	//Get all triggers and store a single instance to allow us to use static functions
	std::vector<TriggerBase*> trigInstances;

	for(auto trigName : inTrigs) {
		auto it = ECSManager::i()->gameTriggers.find(trigName);
		if(it != ECSManager::i()->gameTriggers.end()) {
			trigInstances.push_back(it->second);
		}
	}
	attachedTriggers = trigInstances;
}

bool SystemBase::hasRequired(Entity* ent)
{
	auto compNames = ent->getComponents();
	//Check existence of all required components
	bool isInside = true;
	for(auto reqComp : requiredComps) {
		if(std::find(compNames.begin(), compNames.end(), reqComp) == compNames.end()) {
			isInside = false;
			break;
		}
	}
	return isInside;
}

bool SystemBase::hasEntity(Entity* entToCheck)
{
	return std::find(subbedEntities.begin(), subbedEntities.end(), entToCheck) != subbedEntities.end();
}

bool SystemBase::subscribeEntity(Entity* entToSub) {
	//Check if in subbedEntities
	if(!hasEntity(entToSub)) {
		bool hasReq = hasRequired(entToSub);
		// All components exist within this system, subscribe!
		if(hasReq) {
			subbedEntities.push_back(entToSub);
			return true;
		}
	}
	return false;
}

bool SystemBase::unsubscribeEntity(Entity *entToUnSub) {
	//Check if in subbedEntities
	auto it = std::find(subbedEntities.begin(), subbedEntities.end(), entToUnSub);
	if(it != subbedEntities.end()) {
		//Delete if within
		subbedEntities.erase(it);
		return true;
	}
	return false;
}

std::vector<Entity*> SystemBase::getEntities() const {
	return subbedEntities;
}

std::vector<TriggerBase*> SystemBase::getTriggers() const {
	return attachedTriggers;
}

void SystemBase::updateSystemTriggers() {
	for(auto trig : getTriggers()) {
		trig->runSystemFunction(this);
	}
}

void SystemBase::updateEntityTriggers(Entity *ent) {
	for(auto entTrig : ent->getTriggers()) {
		if(entTrig->getSystemName() == getName()) {
			entTrig->runEntityCheck(this, ent);
		}
	}
}

void SystemBase::subscribeCallback(Entity *entSubbed) {}
void SystemBase::unsubscribeCallback(Entity *entUnsubbed) {}
