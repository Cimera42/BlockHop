//
// Created by Jono on 03/03/2017.
//

#include "TestClass.h"

bool Component::exported = ECSManager::exportComponent<TestClass>("testComponent");

TestClass::TestClass() {}
TestClass::~TestClass() {}

void TestClass::setValues(json inValues) {
    //Will throw something if incorrect/should be caught by ECSManager
    inValues["test"].get<bool>();
}