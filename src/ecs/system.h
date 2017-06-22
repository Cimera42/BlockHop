#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <vector>
#include <string>
#include "entity.h"

class System {
public:
    /*
     * A system is what does the processing for a specific part
     * of the engine. A system is first exported via the ECSManager
     * and then created through the ECSManager.
     */
    System();
    virtual ~System();

    /*
     * create() is used during the creation process of the system
     * by the ECSManager.
     * setRequiredComponents() is used to set the components which
     * are needed for the system to run. Set during creation via
     * the ECSManager.
     * Do not use either of these outside of ECSManager!
     */
    template<typename T>
        static System* create() {return new T; };
    void setRequiredComponents(std::vector<std::string> inComps);

    /*
     * Functions for adding/removing entities from the system.
     * Should only be called from the Entity class.
     */
    bool subscribeEntity(Entity* entToSub);
    bool unsubscribeEntity(Entity* entToUnSub);

    /*
     * Helpers to retrieved subscribed entities
     */
    std::vector<Entity*> getEntities() const;

    /*
     * Updates are called by the engine to run the system.
     */
    virtual void update(double dt) = 0;

private:
    /*
     * Store of the components that are needed to subscribe an
     * entity to a system.
     */
    std::vector<std::string> requiredComps;
    /*
     * List of entities that are subscribed to a system. Used for
     * running the system only for entities that need it.
     */
    std::vector<Entity*> subbedEntities;
    /*
     * Used to export so that ECSManager can see a specific system
     * Usage inside systems's cpp file:
     *      bool System::exported = ECSManager::exportSystem<SystemClass>("systemName");
     */
    //static bool exported;
};

#endif // SYSTEM_H_INCLUDED