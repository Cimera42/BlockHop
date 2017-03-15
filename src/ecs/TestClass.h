//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_CLION_TESTCLASS_H
#define BLOCKHOP_CLION_TESTCLASS_H
#include "component.h"
#include "ecsManager.h"

class TestClass : public Component {
public:
    TestClass();
    ~TestClass();
    void setValues(json inValues);

    int testInt = 10;
};

#endif //BLOCKHOP_CLION_TESTCLASS_H
