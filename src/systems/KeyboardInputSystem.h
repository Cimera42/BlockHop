//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_KEYBOARDINPUTSYSTEM_H
#define BLOCKHOP_KEYBOARDINPUTSYSTEM_H

#include "../ecs/system.h"

class KeyboardInputSystem : public SystemStatics<KeyboardInputSystem>
{
public:
	KeyboardInputSystem();
	~KeyboardInputSystem();

	std::map<int, int> keyList;

	bool isKeyPressed(int keyCode);

	void update(double dt);
};

#endif //BLOCKHOP_KEYBOARDINPUTSYSTEM_H