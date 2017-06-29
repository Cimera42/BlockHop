//
// Created by Jono on 03/03/2017.
//

#include "TestComponent.h"

COMPONENT_EXPORT(TestComponent, "testComponent")

TestComponent::TestComponent() {}
TestComponent::~TestComponent() {}

void TestComponent::setValues(json inValues) {
	//Will throw if incorrect/should automatically be caught by ECSManager
	inValues["test"].get<bool>();
}