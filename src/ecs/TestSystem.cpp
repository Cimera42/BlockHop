//
// Created by Jono on 14/03/2017.
//

#include "TestSystem.h"

bool System::exported = ECSManager::exportSystem<TestSystem>("testSystem");

TestSystem::TestSystem() {}
TestSystem::~TestSystem() {}