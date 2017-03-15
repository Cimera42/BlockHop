#include "entity.h"

Entity::Entity() {}
Entity::~Entity(){}

void Entity::addComponent(Component* comp, std::string compName) {
    subbedComponents.insert(std::make_pair(compName, comp));
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

void Entity::removeComponent(std::string compName) {
    subbedComponents.erase(compName);//TODO test this works
    //After updating our subscribed components, redo system subscription
    subscribeToSystems();
}

void Entity::subscribeToSystems() {
    //TODO implement subscription to systems
    //Check each system for required components
        //Unsubscribe from each system
        //Re check components
        //Re sub if met
}