//
// Created by Jono on 03/07/2018.
//

#include "ApplyForceAction.h"
#include "../ecs/ecsManager.h"
#include "../triggers/ClickedTrigger.h"

ACTION_EXPORT(ApplyForceAction, "applyForceAction");

ApplyForceAction::ApplyForceAction(){};
ApplyForceAction::~ApplyForceAction(){};

void ApplyForceAction::setValues(json inValues) {
	int theForce = inValues["force"].get<int>();
	force = theForce;
}

void ApplyForceAction::runAction(System *s, Entity *e, Trigger *t) {
	auto ct = static_cast<ClickedTrigger*>(t);

	ct->clicked->applyForce(rp3d::Vector3(ct->direction.x,ct->direction.y,ct->direction.z)*force, ct->worldPoint);
}