//
// Created by Jono on 14/03/2017.
//

#include "testSystem.h"

bool System::exported = ECSManager::i()->exportSystem<TestSystem>("testSystem");

TestSystem::TestSystem() {}
TestSystem::~TestSystem() {}

void TestSystem::update() {
    Logger(1)<<"Virtual inheritance test works!";
}