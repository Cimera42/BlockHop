//
// Created by Jono on 14/03/2017.
//

#ifndef BLOCKHOP_CLION_TESTSYSTEM_H
#define BLOCKHOP_CLION_TESTSYSTEM_H

#include "system.h"
#include "ecsManager.h"

class TestSystem : public System {
public:
    TestSystem();
    ~TestSystem();

    int ok = 100;
};


#endif //BLOCKHOP_CLION_TESTSYSTEM_H
