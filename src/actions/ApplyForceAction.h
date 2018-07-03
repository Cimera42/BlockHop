//
// Created by Jono on 03/07/2018.
//

#ifndef BLOCKHOP_APPLYFORCEACTION_H
#define BLOCKHOP_APPLYFORCEACTION_H

#include "../ecs/action.h"

class ApplyForceAction : public Action {
	static bool exported;

public:
	ApplyForceAction();
	~ApplyForceAction();

	int force;

	void setValues(json inValues);
	void runAction(System* s, Entity* e, Trigger* t);
};


#endif //BLOCKHOP_APPLYFORCEACTION_H
