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
	forwardKey 	= convertStringCode(inValues["forward"].get<std::string>());
	backKey 	= convertStringCode(inValues["back"].get<std::string>());
	leftKey 	= convertStringCode(inValues["left"].get<std::string>());
	rightKey 	= convertStringCode(inValues["right"].get<std::string>());
	upKey 		= convertStringCode(inValues["up"].get<std::string>());
	downKey 	= convertStringCode(inValues["down"].get<std::string>());
}

int KeyboardControlComponent::convertStringCode(std::string val)
{
	if(val.size() > 0)
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
int KeyboardControlComponent::getRightKey() const {	return rightKey; }
int KeyboardControlComponent::getUpKey() const { return upKey; }
int KeyboardControlComponent::getDownKey() const { return downKey; }
