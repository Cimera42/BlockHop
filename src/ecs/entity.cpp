#include "entity.h"
#include "ecsManager.h"

Entity::Entity() {}
Entity::~Entity(){}

void Entity::addComponent(Component* comp) {
    subbedComponents.push_back(comp);
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

void Entity::removeComponent(std::string compName) {
    std::vector<Component*>::iterator it = std::find_if(subbedComponents.begin(), subbedComponents.end(), [&compName](Component*& o) {
        return (o->getName() == compName);
    });

    if (it != subbedComponents.end())
        subbedComponents.erase(it);
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

std::vector<std::string> Entity::getComponents() {
    std::vector<std::string> compNames;
    for(auto &comp : subbedComponents) {
        compNames.push_back(comp->getName());
    }
    return compNames;
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