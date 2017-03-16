#include "entity.h"
#include "ecsManager.h"

Entity::Entity() {}
Entity::~Entity(){}

void Entity::addComponent(std::string compName, Component* comp) {
    subbedComponents.insert(std::make_pair(compName, comp));
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

void Entity::removeComponent(std::string compName) {
    subbedComponents.erase(compName);//TODO test this works
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

std::vector<std::string> Entity::getComponents() {
    std::vector<std::string> compNames;
    for(auto &comp : subbedComponents) {
        compNames.push_back(comp.first);
    }
    return compNames;
}

Component* Entity::getComponent(std::string compName) {
    auto it = subbedComponents.find(compName);
    if (it != subbedComponents.end())
        return it->second;
    return nullptr;
}

void Entity::subscribeToSystems() {
    //Move through each system
    for(auto &sys : ECSManager::i()->gameSystems) {
        //First unsubscribe from the system
        auto sysPtr = sys.second;
        sysPtr->unsubscribeEntity(this);

        //Then attempt to subscribe.
        //A false return type means that either the entity is already subbed
        //or doesn't meet the requirements. However we can assume since we just
        //unsubbed that its the latter
        if(sysPtr->subscribeEntity(this)) {
            Logger(1) << "Entity successfully subscribed to "<<sys.first;
        }
    }
}