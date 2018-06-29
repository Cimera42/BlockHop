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
	 * setAttachedTriggers() is used to attach which triggers are
	 * run by this system, also set by ECSManager.
	 * Do not use either of these outside of ECSManager!
	 */
	template<typename T>
		static System* create() {return new T; };
	void setRequiredComponents(std::vector<std::string> inComps);
	std::vector<std::string> getRequiredComponents();
	void setAttachedTriggers(std::vector<std::string> inTrigs);

	/*
	 * Check if entity has all required components
	 */
	bool hasRequired(Entity *ent);
	
	/*
	 * Check if an entity is subscribed to the system
	 */
	bool hasEntity(Entity *entToCheck);

	/*
	 * Functions for adding/removing entities from the system.
	 * Should only be called from the Entity class.
	 */
	bool subscribeEntity(Entity* entToSub);
	bool unsubscribeEntity(Entity* entToUnSub);

	/*
	 * Virtual functions called when entity added to the system.
	 */
	virtual void subscribeCallback(Entity *entSubbed);
	virtual void unsubscribeCallback(Entity *entUnsubbed);

	/*
	 * Helpers to retrieved subscribed entities
	 */
	std::vector<Entity*> getEntities() const;

	/*
	 * Helper to retrieve attached triggers by name
	 */
	std::vector<Trigger*> getTriggers() const;


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
	 * TODO is there a better way to do this?
	 * Store a single instance triggers that would run inside a system.
	 */
	std::vector<Trigger*> attachedTriggers;
	/*
	 * List of entities that are subscribed to a system. Used for
	 * running the system only for entities that need it.
	 */
	std::vector<Entity*> subbedEntities;
	/*
	 * Used to export so that ECSManager can see a specific system
	 * Usage inside systems's cpp file:
	 *	  bool System::exported = ECSManager::exportSystem<SystemClass>("systemName");
	 */
	//static bool exported;
};

#endif // SYSTEM_H_INCLUDED