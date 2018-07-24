//
// Created by Jono on 07/07/2018.
//

#include "TimeoutComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(TimeoutComponent, "timeoutComponent")

TimeoutComponent::TimeoutComponent() {
	start = std::chrono::steady_clock::now();
}
TimeoutComponent::~TimeoutComponent() {}

void TimeoutComponent::setValues(json inValues) 
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	if(inValues.find("duration") != inValues.end()) {
		duration = inValues["duration"].get<long>();
	}
}

void TimeoutComponent::setDuration(long milliseconds) {
	duration = milliseconds;
	end = start + std::chrono::milliseconds(duration);
}

void TimeoutComponent::reset() {
	start = std::chrono::steady_clock::now();
	end = start + std::chrono::milliseconds(duration);
}

bool TimeoutComponent::hasTimedOut() {
	return std::chrono::steady_clock::now() > end;
}

void TimeoutComponent::setCallback(std::function<void(Entity* e)> func) {
	boundFunc = func;
}

void TimeoutComponent::runCallback(Entity *e) {
	boundFunc(e);
}