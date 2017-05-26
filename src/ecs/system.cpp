#include "system.h"

System::System() {}
System::~System() {}

void System::setRequiredComponents(std::vector<std::string> inComps) {
    requiredComps = inComps;
}

bool System::subscribeEntity(Entity* entToSub) {
    //Check if in subbedEntities
    if(!(std::find(subbedEntities.begin(), subbedEntities.end(), entToSub) != subbedEntities.end())) {
        auto compNames = entToSub->getComponents();
        //Not inside, check for required components
        bool isInside = true;
        for(auto reqComp : requiredComps) {
            if(std::find(compNames.begin(), compNames.end(), reqComp) == compNames.end()) {
                isInside = false;
            }
        }
        // All components exist within this system, subscribe!
        if(isInside) {
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

void System::update(double dt) {}