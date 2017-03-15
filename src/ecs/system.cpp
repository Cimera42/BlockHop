#include "system.h"

System::System() {}
System::~System() {}

void System::setRequiredComponents(std::vector<std::string> inComps) {
    requiredComps = inComps;
}

void System::subscribeEntity(Entity* entToSub) {
    //Check if in subbedEntities
    if(!(std::find(subbedEntities.begin(), subbedEntities.end(), entToSub) != subbedEntities.end())) {
        //Not inside, add
        subbedEntities.push_back(entToSub);
    }
}

void System::unsubscribeEntity(Entity *entToUnSub) {
    //Check if in subbedEntities
    auto it = std::find(subbedEntities.begin(), subbedEntities.end(), entToUnSub);
    if(it != subbedEntities.end()) {
        //Delete if within
        subbedEntities.erase(it);
    }
}