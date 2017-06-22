//
// Created by Jono on 14/03/2017.
//

#include "TestSystem.h"

SYSTEM_EXPORT(TestSystem, "testSystem")

TestSystem::TestSystem() {

}
TestSystem::~TestSystem() {}

void TestSystem::update(double dt) {
    //Logger(1)<<"Virtual inheritance test works!";
    for(auto entity : getEntities())
    {

    }
}