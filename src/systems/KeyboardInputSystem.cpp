//
// Created by Tim on 25/05/2017.
//

#include "KeyboardInputSystem.h"
#include "../ecs/ecsManager.h"
#include "../window.h"
#include "../components/TransformComponent.h"
#include "../components/KeyboardControlComponent.h"

SYSTEM_EXPORT(KeyboardInputSystem, "keyboardInputSystem")

extern Window* window;
void keyboardInputEvent(GLFWwindow* inWindow, int keyCode, int scanCode, int action, int modifiers);

KeyboardInputSystem::KeyboardInputSystem() 
{
	glfwSetKeyCallback(window->glfwWindow, keyboardInputEvent);
}
KeyboardInputSystem::~KeyboardInputSystem() {}

void KeyboardInputSystem::update(double dt) 
{
    for(auto entity : getEntities())
    {
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
		KeyboardControlComponent* keyboardControl = entity->getComponent<KeyboardControlComponent>("keyboardControlComponent");

		glm::vec3 displaced = transform->getPosition();
        if(isKeyPressed(keyboardControl->getForwardKey()))
			displaced += transform->getForward() * ((float)dt) * 5.0f;
		
        if(isKeyPressed(keyboardControl->getBackKey()))
			displaced -= transform->getForward() * ((float)dt) * 5.0f;
		
        if(isKeyPressed(keyboardControl->getLeftKey()))
			displaced += transform->getRight() * ((float)dt) * 5.0f;
		
        if(isKeyPressed(keyboardControl->getRightKey()))
			displaced -= transform->getRight() * ((float)dt) * 5.0f;
		
        if(isKeyPressed(keyboardControl->getUpKey()))
			displaced += glm::vec3(0, 1, 0) * ((float)dt) * 5.0f;
		
        if(isKeyPressed(keyboardControl->getDownKey()))
			displaced -= glm::vec3(0, 1, 0) * ((float)dt) * 5.0f;
		
		transform->setPosition(displaced);
		Logger(1) << displaced;
    }
}

bool KeyboardInputSystem::isKeyPressed(int keyCode)
{
	std::map<int, int>::iterator fi = keyList.find(keyCode);
	if(fi != keyList.end())
	{
		if(fi->second == GLFW_PRESS || fi->second == GLFW_REPEAT)
		{
			return true;
		}
	}
	return false;
}

void keyboardInputEvent(GLFWwindow* inWindow, int keyCode, int scanCode, int action, int modifiers)
{
	KeyboardInputSystem* keyboardSystem = ECSManager::i()->findSystem<KeyboardInputSystem>("keyboardInputSystem");
	keyboardSystem->keyList[keyCode] = action;
}