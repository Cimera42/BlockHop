//
// Created by Jono on 03/03/2017.
//

#ifndef BLOCKHOP_TESTCOMPONENT_H
#define BLOCKHOP_TESTCOMPONENT_H

#include "../ecs/component.h"

class TestComponent : public Component
{
	static bool exported;
public:
	void setValues(const json &inValues) override;

	int testInt = 10;
};

#endif //BLOCKHOP_TESTCOMPONENT_H
