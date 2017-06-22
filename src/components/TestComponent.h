//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_TESTCOMPONENT_H
#define BLOCKHOP_TESTCOMPONENT_H

#include "../ecs/component.h"
#include "../ecs/ecsManager.h"

class TestComponent : public Component
{
    static bool exported;
public:
    TestComponent();
    ~TestComponent();
    void setValues(json inValues);

    int testInt = 10;
};

#endif //BLOCKHOP_TESTCOMPONENT_H
