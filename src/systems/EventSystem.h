//
// Created by Jono on 07/07/2018.
//

#ifndef BLOCKHOP_EVENTSYSTEM_H
#define BLOCKHOP_EVENTSYSTEM_H

#include "../ecs/system.h"

class EventSystem : public SystemStatics<EventSystem>
{
public:
	EventSystem();
	~EventSystem();

	//TODO - generic event dispatch/recieve for times
	// when we need cross - entity communication
	// but we have no real idea what those entities are
	// Essentially a broadcaster - probably based on
	// components/triggers that are attached to an entity

	void update(double dt);
};

#endif //BLOCKHOP_EVENTSYSTEM_H