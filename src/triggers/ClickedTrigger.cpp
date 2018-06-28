//
// Created by Jono on 28/06/2018.
//

#include "ClickedTrigger.h"
#include "../ecs/ecsManager.h"
#include "../systems/MouseButtonSystem.h"
#include "../systems/PhysicsSystem.h"
#include "../components/TransformComponent.h"
#include <GLFW/glfw3.h>

TRIGGER_EXPORT(ClickedTrigger, "clickedTrigger");

int ClickedTrigger::testSystemValue = 0;

ClickedTrigger::ClickedTrigger(){}
ClickedTrigger::~ClickedTrigger(){}

void ClickedTrigger::setValues(json inValues) {
	testEntityValue = inValues["testData"].get<int>();
}

// Class WorldRaycastCallback
class MyCallbackClass : public rp3d::RaycastCallback {

public:
	glm::vec3 dir;
	float forceValue;
	MyCallbackClass(glm::vec3 inDir, float inForceValue) {
		dir = inDir;
		forceValue = inForceValue;
	}

	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info)
	{
		if(info.body->getType() == rp3d::DYNAMIC)
		{
			rp3d::RigidBody* rb = (reactphysics3d::RigidBody *) info.body;
			rb->applyForce(rp3d::Vector3(dir.x,dir.y,dir.z)*forceValue, info.worldPoint);
		}

		// Return a fraction of 1.0 to gather all hits
		return rp3d::decimal(1.0);
	}
};

void ClickedTrigger::runSystemFunction(System* sys) {
	PhysicsSystem* s = static_cast<PhysicsSystem*>(sys);

	MouseButtonSystem* mouseButtonSystem = ECSManager::i()->findSystem<MouseButtonSystem>("mouseButtonSystem");
	int b = 0;
	if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		b = 1;
	else if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		b = -1;
	if(b != 0)
	{
		Entity* cameraEntity = ECSManager::i()->findEntity("Camera");
		TransformComponent* cameraTransform = cameraEntity->getComponent<TransformComponent>("transformComponent");
		glm::vec3 startGlm = cameraTransform->getPosition();
		glm::vec3 direction = cameraTransform->getForward();
		glm::vec3 mDirection = direction*20.0f;

		rp3d::Vector3 start(startGlm.x,startGlm.y,startGlm.z);
		rp3d::Vector3 end = start + rp3d::Vector3(mDirection.x,mDirection.y,mDirection.z);
		rp3d::Ray ray(start,end);

		//TODO This would be what actions are for but yeah
		/*TODO Basically you would store which entity it has hit, then inside runEntityCheck - check if its that entity
		 and then run appropriate actions from here
		 */
		MyCallbackClass callback(direction*((float)b), testEntityValue); //this needs to be on entity level
		s->getDyanmicWorld()->raycast(ray, &callback);
	}

	testSystemValue = 4;
}

void ClickedTrigger::runEntityCheck(System* sys, Entity* ent) {

}