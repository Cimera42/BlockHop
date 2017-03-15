#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <vector>
#include <string>
#include "component.h"

class Entity {
public:
    Entity();
    ~Entity();

    void addComponent(Component* comp, std::string compName);
    void removeComponent(std::string compName);

private:
    void subscribeToSystems();
    std::map<std::string, Component*> subbedComponents;
};

#endif // ENTITY_H_INCLUDED