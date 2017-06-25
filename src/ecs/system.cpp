#include "system.h"

System::System() {}
System::~System() {}

void System::setRequiredComponents(std::vector<std::string> inComps) {
    requiredComps = inComps;
}

bool System::hasRequired(Entity* ent)
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

bool System::hasEntity(Entity* entToCheck)
{
	return std::find(subbedEntities.begin(), subbedEntities.end(), entToCheck) != subbedEntities.end();
}

bool System::subscribeEntity(Entity* entToSub) {
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

bool System::unsubscribeEntity(Entity *entToUnSub) {
    //Check if in subbedEntities
    auto it = std::find(subbedEntities.begin(), subbedEntities.end(), entToUnSub);
    if(it != subbedEntities.end()) {
        //Delete if within
        subbedEntities.erase(it);
        return true;
    }
    return false;
}

std::vector<Entity*> System::getEntities() const {
    return subbedEntities;
}

void System::subscribeCallback(Entity *entSubbed) {}
void System::unsubscribeCallback(Entity *entUnsubbed) {}
