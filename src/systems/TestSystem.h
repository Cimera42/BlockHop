//
// Created by Jono on 14/03/2017.
//

#ifndef BLOCKHOP_TESTSYSTEM_H
#define BLOCKHOP_TESTSYSTEM_H

#include "../ecs/system.h"
#include "../ecs/ecsManager.h"

class TestSystem : public System
{
    static bool exported;
public:
    TestSystem();
    ~TestSystem();

    int ok = 100;
    void update(double dt);
};

#endif //BLOCKHOP_TESTSYSTEM_H
