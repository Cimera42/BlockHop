//
// Created by Tim on 25/05/2017.
//

#include "MouseControlComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(MouseControlComponent, "mouseControlComponent")

MouseControlComponent::MouseControlComponent() {}
MouseControlComponent::~MouseControlComponent() {}

void MouseControlComponent::setValues(json inValues)
{
	//Will throw if incorrect/should automatically be caught by ECSManager
	xSensitivity = inValues["xSensitivity"];
	ySensitivity = inValues["ySensitivity"];
}

double MouseControlComponent::getXSensitivity() const
{
	return xSensitivity;
}

double MouseControlComponent::getYSensitivity() const
{
	return ySensitivity;
}
