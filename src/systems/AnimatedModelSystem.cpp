//
// Created by Tim on 25/05/2017.
//

#include "AnimatedModelSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/AnimatedModelComponent.h"
#include "../openGLFunctions.h"
#include "KeyboardInputSystem.h"
#include <glm/gtx/transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

SYSTEM_EXPORT(AnimatedModelSystem, "animatedModelSystem")

AnimatedModelSystem::AnimatedModelSystem() {}

AnimatedModelSystem::~AnimatedModelSystem() {}

void AnimatedModelSystem::update(double dt)
{
	KeyboardInputSystem* keyInput = ECSManager::get().findSystem<KeyboardInputSystem>();

	updateSystemTriggers();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		AnimatedModelComponent* animatedModel = entity->getComponent<AnimatedModelComponent>();

		//Temporary test animation switching
		if(keyInput)
		{
			if(keyInput->isKeyPressed(GLFW_KEY_1))
				animatedModel->playAnimation("Armature|Grip");

			else if(keyInput->isKeyPressed(GLFW_KEY_2))
				animatedModel->playAnimation("Armature|GunFingers");

			else if(keyInput->isKeyPressed(GLFW_KEY_3))
				animatedModel->playAnimation("Armature|Chop");
		}
		//Animate model
		animatedModel->transformNodes((float) dt);
	}
}