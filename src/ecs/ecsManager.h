//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_CLION_ECSMANAGER_H
#define BLOCKHOP_CLION_ECSMANAGER_H

#include "entity.h"
#include "component.h"
#include "system.h"
#include <string>
#include <map>
#include <vector>
#include "../logger.h"

typedef Component* (*ComponentFactoryPtr)();
typedef System* (*SystemFactoryPtr)();

class ECSManager {
public:
    ECSManager();
    ~ECSManager();

    /*int findComponent(std::string name); See if we can input a classType and return a string? ie from the map
    System* findSystem(std::string name);
    Entity* findEntity(std::string name);*/

    //addComponentToEntity(Entity* ent, std::string comp);
    //deleteComponentFromEntity(Entity* ent, std::string comp);

    static Component* createComponent(std::string name, json compData);
    static System* createSystem(std::string name, std::vector<std::string> compsNeeded);
    //TODO consider merging compsToSub and compsData into one larger json which we process?
    static Entity* createEntity(std::string name, std::vector<std::string> compsToSub, std::vector<json> compsData);

    //Used to generate references to systems by string
    template<typename T>
    static bool exportSystem(std::string name) {
        SystemFactoryPtr sysFunc = &System::create<T>;
        gameSystemExports.insert(std::make_pair(name, sysFunc));
        return true;
    }

    //Used to generate references to components, allowing for creation by string
    template<typename T>
    static bool exportComponent(std::string name) {
        ComponentFactoryPtr compFunc = &Component::create<T>;
        gameComponentExports.insert(std::make_pair(name, compFunc));
        return true;
    }

    /*
     * ECS data accessible to public
     */
    static std::map<std::string, Entity*> gameEntities;
    static std::map<std::string, System*> gameSystems;

private:
    //Holds references to exports for use in creation
    static std::map<std::string, SystemFactoryPtr> gameSystemExports;
    static std::map<std::string, ComponentFactoryPtr> gameComponentExports;
};

#endif //BLOCKHOP_CLION_ECSMANAGER_H
