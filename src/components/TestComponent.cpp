//
// Created by Jono on 03/03/2017.
//

#include "TestComponent.h"

COMPONENT_EXPORT(TestComponent, "testComponent")
void TestComponent::setValues(const json &inValues) {
	//Will throw if incorrect/should automatically be caught by ECSManager
	inValues["test"].get<bool>();
}