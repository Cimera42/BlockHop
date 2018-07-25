//
// Created by Jono on 07/07/2018.
//

#ifndef BLOCKHOP_TIMEOUTCOMPONENT_H
#define BLOCKHOP_TIMEOUTCOMPONENT_H

#include "../ecs/component.h"
#include "../ecs/entity.h"
#include <chrono>

//TODO - can only have one timeout at a time per entity, allow for more!
//TODO - also need to have a way of preserving variables at time of calling...
//TODO - also make more dynamic with slow mo?
class TimeoutComponent : public ComponentStatics<TimeoutComponent>
{
	std::function<void(Entity* e)> boundFunc;
	long duration;
	std::chrono::time_point<std::chrono::steady_clock> start, end; //Everything is in seconds
public:
	TimeoutComponent();
	~TimeoutComponent();
	void setValues(json inValues);

	void setDuration(long milliseconds);
	void reset();
	bool hasTimedOut();

	void setCallback(std::function<void(Entity* e)> func);
	void runCallback(Entity* e);

	friend std::ostream &operator<< (std::ostream &os, TimeoutComponent const &c) {
		os << "TimeoutComponent: ";
		os << "(" << c.duration << ")";
		return os;
	}
};

#endif //BLOCKHOP_TIMEOUTCOMPONENT_H