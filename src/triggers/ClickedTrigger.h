//
// Created by Jono on 28/06/2018.
//

#ifndef BLOCKHOP_CLICKEDTRIGGER_H
#define BLOCKHOP_CLICKEDTRIGGER_H

#include "../ecs/trigger.h"

class ClickedTrigger : public Trigger {
	static bool exported;

public:
	ClickedTrigger();
	~ClickedTrigger();
	void setValues(json inValues);

	static int testSystemValue;
	int testEntityValue;

	void runSystemFunction(System* s);
	void runEntityCheck(System* s, Entity* e);
};

#endif //BLOCKHOP_CLICKEDTRIGGER_H
