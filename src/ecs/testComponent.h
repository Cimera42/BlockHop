//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_CLION_TESTCOMPONENT_H
#define BLOCKHOP_CLION_TESTCOMPONENT_H
#include "component.h"
#include "ecsManager.h"

class TestComponent : public Component {
public:
    TestComponent();
    ~TestComponent();
    void setValues(json inValues);

    int testInt = 10;
};

#endif //BLOCKHOP_CLION_TESTCOMPONENT_H
