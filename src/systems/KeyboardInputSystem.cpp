//
// Created by Tim on 25/05/2017.
//

#include "KeyboardInputSystem.h"
#include "../ecs/ecsManager.h"
#include "../window.h"
#include "../components/TransformComponent.h"

SYSTEM_EXPORT(KeyboardInputSystem, "keyboardInputSystem")

extern Window* window;
KeyboardInputSystem::KeyboardInputSystem() {}
KeyboardInputSystem::~KeyboardInputSystem() {}

void KeyboardInputSystem::update(double dt) 
{
	Logger(1) << "----------" << dt;
    for(auto entity : getEntities())
    {
		TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");

		glm::vec3 displaced = transform->getPosition();
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
			displaced += transform->getForward() * ((float)dt) * 5.0f;
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
			displaced -= transform->getForward() * ((float)dt) * 5.0f;
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
			displaced += transform->getRight() * ((float)dt) * 5.0f;
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
			displaced -= transform->getRight() * ((float)dt) * 5.0f;
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			displaced += glm::vec3(0, 1, 0) * ((float)dt) * 5.0f;
        if(glfwGetKey(window->glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			displaced -= glm::vec3(0, 1, 0) * ((float)dt) * 5.0f;
		
		transform->setPosition(displaced);
    }
}