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
		auto transform = entity->getComponent<TransformComponent>("transformComponent");
		auto keyboardControl = entity->getComponent<KeyboardControlComponent>("keyboardControlComponent");

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
			displaced += transform->getUp() * ((float)dt) * 5.0f;

		if(isKeyPressed(keyboardControl->getDownKey()))
			displaced -= transform->getUp() * ((float)dt) * 5.0f;

		transform->setPosition(displaced);


		if(isKeyPressed(GLFW_KEY_R))
		{
			std::vector<std::string> comps = {"transformComponent",
											  "physicsComponent",
											  "animatedModelComponent"};

			json tj = {{"position",{{"x",2.5},{"y",10},{"z",0}}},
					   {"rotation",{{"w",1},{"x",0},{"y",0},{"z",0}}},
					   {"scale",{{"x",1},{"y",1},{"z",1}}}};
			json pj = {
				{"mode","dynamic"},
		  		{"colliderShape","cube"},
				{"halfWidth",1},
				{"halfHeight",1},
				{"halfDepth",1},
				{"mass",5}
			};
			json aj = {{"filename","./assets/models/ColourfulCube/framedCube.fbx"}};
			std::vector<json> compData = {tj,pj,aj};
			ECSManager::i()->createEntity("projectile", comps, compData);
		}
	}
}

bool KeyboardInputSystem::isKeyPressed(int keyCode)
{
	auto fi = keyList.find(keyCode);
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