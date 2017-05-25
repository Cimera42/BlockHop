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

}