//
// Created by Jono on 07/07/2018.
//

#ifndef BLOCKHOP_TIMESYSTEM_H
#define BLOCKHOP_TIMESYSTEM_H

#include "../ecs/system.h"

class TimeSystem : public System<TimeSystem>
{
public:
	TimeSystem();
	~TimeSystem();

	void update(double dt);
};

#endif //BLOCKHOP_TIMESYSTEM_H