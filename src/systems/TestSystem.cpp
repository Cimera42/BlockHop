//
// Created by Jono on 14/03/2017.
//

#include "TestSystem.h"
#include "../ecs/ecsManager.h"

SYSTEM_EXPORT(TestSystem, "testSystem")

TestSystem::TestSystem() {

}
TestSystem::~TestSystem() {}

void TestSystem::update(double dt, double alpha) {
	//Logger()<<"Virtual inheritance test works!";

	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

	}
}