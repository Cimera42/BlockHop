#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <vector>
#include <string>
#include "component.h"

class Entity {
public:
    /*
     * A entity holds references to components which in turn allow
     * it to be subscribed to specific systems. It is basically the
     * connecting block which we worry about when creating a game.
     * An entity does not need to exported as there are no derived
     * types, instead they are created through the ECSManager.
     */
    Entity();
    ~Entity();

    /*
     * Functions for adding/removing components from entities.
     * Should be used to enable/disable functionality of an
     * entity.
     */
    void addComponent(std::string compName, Component* comp);
    void removeComponent(std::string compName);

    /*
     * Helpers for retrieving components attached to an entity
     */
    std::vector<std::string> getComponents();
    Component* getComponent(std::string compName);
private:
    void subscribeToSystems();
    std::map<std::string, Component*> subbedComponents;
};

#endif // ENTITY_H_INCLUDED