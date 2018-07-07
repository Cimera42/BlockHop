//
// Created by Jono on 07/07/2018.
//

#ifndef BLOCKHOP_TIMEOUTCOMPONENT_H
#define BLOCKHOP_TIMEOUTCOMPONENT_H

#include "../ecs/component.h"

class TimeoutComponent : public Component
{
	static bool exported;
public:
	TimeoutComponent();
	~TimeoutComponent();
	void setValues(json inValues);

	friend std::ostream &operator<< (std::ostream &os, TimeoutComponent const &c) {
		os << "TimeoutComponent";
		return os;
	}
};

#endif //BLOCKHOP_TIMEOUTCOMPONENT_H