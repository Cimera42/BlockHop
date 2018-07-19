//
// Created by Tim on 25/05/2017.
//

#include "KeyboardControlComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(KeyboardControlComponent, "keyboardControlComponent")

KeyboardControlComponent::KeyboardControlComponent() {}
KeyboardControlComponent::~KeyboardControlComponent() {}

void KeyboardControlComponent::setValues(json inValues)
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	forwardKey 	= convertStringCode(inValues["forward"]);
	backKey 	= convertStringCode(inValues["back"]);
	leftKey 	= convertStringCode(inValues["left"]);
	rightKey 	= convertStringCode(inValues["right"]);
	upKey 		= convertStringCode(inValues["up"]);
	downKey 	= convertStringCode(inValues["down"]);
}

int KeyboardControlComponent::convertStringCode(std::string val)
{
	if(!val.empty())
	{
		if(val.size() > 1)
			return std::stoi(val);
		else
			return val.at(0);
	}
	else
		return 0;
}

int KeyboardControlComponent::getForwardKey() const { return forwardKey; }
int KeyboardControlComponent::getBackKey() const { return backKey; }
int KeyboardControlComponent::getLeftKey() const { return leftKey; }
int KeyboardControlComponent::getRightKey() const { return rightKey; }
int KeyboardControlComponent::getUpKey() const { return upKey; }
int KeyboardControlComponent::getDownKey() const { return downKey; }
