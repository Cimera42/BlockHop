//
// Created by Tim on 25/05/2017.
//

#include "KeyboardInputSystem.h"
#include "../ecs/ecsManager.h"
#include "../window.h"
#include "../components/TransformComponent.h"
#include "../components/KeyboardControlComponent.h"
#include "../triggers/ClickedTrigger.h"

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
	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		auto transform = entity->getComponent<TransformComponent>();
		auto keyboardControl = entity->getComponent<KeyboardControlComponent>();

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
	}

	if(isKeyPressed(GLFW_KEY_R))
	{
		std::vector<std::string> comps = {"transformComponent",
										  "physicsComponent",
										  "animatedModelComponent"};
		std::vector<std::string> trigs = {"clickedTrigger"};

		json tj = {
			{"position",{2.5, 10, 0}},
			{"rotation",{1, 0, 0, 0}},
			{"scale",{1, 1, 1}}
		};
		json pj = {
			{"colliderShape","cube"},
			{"halfDimensions", {1, 1, 1}},
			{"mass", 0.5}
		};
		json aj = {{"filename","./assets/models/ColourfulCube/framedCube.fbx"}};
		std::vector<json> compData = {tj,pj,aj};

		json ct = {{"force", rand() % 100 + 1}};
		std::vector<json> trigData = {ct};

		ECSManager::i()->createEntity("projectile", comps, compData, trigs, trigData);
	}
	if(isKeyPressed(GLFW_KEY_T))
	{
		std::string entname = "RaycastIndicator";
		entname += std::to_string(ClickedTrigger::indicatorAccumulator);
		auto indicator = ECSManager::i()->findEntity(entname);
		if(indicator)
		{
			ClickedTrigger::indicatorAccumulator++;
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
	KeyboardInputSystem* keyboardSystem = ECSManager::i()->findSystem<KeyboardInputSystem>();
	keyboardSystem->keyList[keyCode] = action;
}