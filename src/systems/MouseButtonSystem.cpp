//
// Created by Tim on 25/06/2017.
//

#include <GLFW/glfw3.h>
#include "MouseButtonSystem.h"
#include "../ecs/ecsManager.h"
#include "../window.h"

SYSTEM_EXPORT(MouseButtonSystem, "mouseButtonSystem")

extern Window* window;
void mouseButtonEvent(GLFWwindow* inWindow, int buttonCode, int action, int modifiers);

MouseButtonSystem::MouseButtonSystem() 
{
	glfwSetMouseButtonCallback(window->glfwWindow, mouseButtonEvent);
}
MouseButtonSystem::~MouseButtonSystem() {}

void MouseButtonSystem::update(double dt) 
{
	/*for(auto trig : getTriggers()) {
		trig->runSystemFunction(this);
	}

	for(auto entity : getEntities())
	{
		for(auto entTrig : entity->getTriggers()) {
			if(entTrig->getSystemName() == "timeSystem") {
				entTrig->runEntityCheck(this, entity);
			}
		}

		//XComponent* x = entity->getComponent<XComponent>("xComponent");
	}*/
}

bool MouseButtonSystem::isButtonPressed(int buttonCode)
{
	auto fi = buttonList.find(buttonCode);
	if(fi != buttonList.end())
	{
		if(fi->second == GLFW_PRESS || fi->second == GLFW_REPEAT)
		{
			return true;
		}
	}
	return false;
}

void mouseButtonEvent(GLFWwindow* inWindow, int buttonCode, int action, int modifiers)
{
	MouseButtonSystem* mouseButtonSystem = ECSManager::i()->findSystem<MouseButtonSystem>("mouseButtonSystem");
	mouseButtonSystem->buttonList[buttonCode] = action;
}