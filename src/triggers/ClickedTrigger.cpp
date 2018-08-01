//
// Created by Jono on 28/06/2018.
//

#include "ClickedTrigger.h"
#include "../ecs/ecsManager.h"
#include "../systems/MouseButtonSystem.h"
#include "../components/TransformComponent.h"
#include "../components/TimeoutComponent.h"
#include <GLFW/glfw3.h>

TRIGGER_EXPORT(ClickedTrigger, "clickedTrigger");

ClickedTrigger::ClickedTrigger(){
	// Register all functions that handle actions
//	addAction("box2", EXACT, static_cast<RunTrigFunc>(&ClickedTrigger::runBox2));
}
ClickedTrigger::~ClickedTrigger(){}

void ClickedTrigger::setValues(json inValues) {
	int theForce = inValues["force"].get<int>();
	force = theForce;
}

void ClickedTrigger::runSystemFunction(SystemBase* sys) {
	PhysicsSystem* s = static_cast<PhysicsSystem*>(sys);
}

bool ClickedTrigger::entityCheck(SystemBase* sys, Entity* ent) {
	PhysicsSystem* s = static_cast<PhysicsSystem*>(sys);

	return false;
}