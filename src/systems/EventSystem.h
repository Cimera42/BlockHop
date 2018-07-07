//
// Created by Jono on 07/07/2018.
//

#ifndef BLOCKHOP_EVENTSYSTEM_H
#define BLOCKHOP_EVENTSYSTEM_H

#include "../ecs/system.h"

class EventSystem : public System 
{
	static bool exported;
public:
	EventSystem();
	~EventSystem();

	void update(double dt);
};

#endif //BLOCKHOP_EVENTSYSTEM_H