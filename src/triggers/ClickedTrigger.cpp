//
// Created by Jono on 28/06/2018.
//

#include "ClickedTrigger.h"
#include "../ecs/ecsManager.h"
#include "../systems/MouseButtonSystem.h"
#include "../components/TransformComponent.h"
#include "../components/TimeoutComponent.h"
#include <GLFW/glfw3.h>

TRIGGER_EXPORT(ClickedTrigger, "clickedTrigger");

rp3d::RigidBody* ClickedTrigger::clicked;
rp3d::Vector3 ClickedTrigger::worldPoint;
glm::vec3 ClickedTrigger::direction;

ClickedTrigger::ClickedTrigger(){
	// Register all functions that handle actions
	addAction("box2", EXACT, static_cast<RunTrigFunc>(&ClickedTrigger::runBox2));
	addAction("box", EXACT, static_cast<RunTrigFunc>(&ClickedTrigger::runBox));
}
ClickedTrigger::~ClickedTrigger(){}

void ClickedTrigger::setValues(json inValues) {
	int theForce = inValues["force"].get<int>();
	force = theForce;
}

// Class WorldRaycastCallback
class MyCallbackClass : public rp3d::RaycastCallback {

public:
	MyCallbackClass() {}

	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info)
	{
		if(info.body->getType() == rp3d::DYNAMIC)
		{
			rp3d::RigidBody* rb = (reactphysics3d::RigidBody *) info.body;
			ClickedTrigger::clicked = rb;
			ClickedTrigger::worldPoint = info.worldPoint;
		}

		// Return a fraction of 1.0 to gather all hits
		return rp3d::decimal(1.0);
	}
};

void ClickedTrigger::runSystemFunction(SystemBase* sys) {
	PhysicsSystem* s = static_cast<PhysicsSystem*>(sys);

	MouseButtonSystem* mouseButtonSystem = ECSManager::get().findSystem<MouseButtonSystem>("mouseButtonSystem");
	int b = 0;
	if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		b = 1;
	else if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		b = -1;
	if(b != 0)
	{
		Entity* cameraEntity = ECSManager::get().findEntity("Camera");
		TransformComponent* cameraTransform = cameraEntity->getComponent<TransformComponent>();
		glm::vec3 startGlm = cameraTransform->getPosition();
		glm::vec3 camDirection = cameraTransform->getForward();
		glm::vec3 mDirection = camDirection*20.0f;

		rp3d::Vector3 start(startGlm.x,startGlm.y,startGlm.z);
		rp3d::Vector3 end = start + rp3d::Vector3(mDirection.x,mDirection.y,mDirection.z);
		rp3d::Ray ray(start,end);

		direction = camDirection*(float)b;
		MyCallbackClass callback;
		s->dynamicsWorld->raycast(ray, &callback);
	} else {
		clicked = nullptr;
	}
}

rp3d::RigidBody* lastClicked; //Just for demo - wont work in real game
void testCallback(Entity *e) {
	auto trigger = e->getTrigger<ClickedTrigger>("clickedTrigger");
	//std::cout << trigger->clicked << std::endl;
	lastClicked->applyForce(rp3d::Vector3(0,1,0)*trigger->force, trigger->worldPoint);
}

bool ClickedTrigger::entityCheck(SystemBase* sys, Entity* ent) {
	PhysicsSystem* s = static_cast<PhysicsSystem*>(sys);

	rp3d::RigidBody* rb = s->findRigidBody(ent);
	if(rb) {
		if (rb->getType() == rp3d::DYNAMIC) {
			if(clicked == rb) {
				return true;
			}
		}
	}
	return false;
}

void ClickedTrigger::runBox2(SystemBase* sys, Entity* ent) {
	auto time = static_cast<TimeoutComponent*>(ECSManager::get().createComponent("timeoutComponent", json {}));
	time->setDuration(3000);
	time->setCallback(testCallback);
	ent->addComponent(time);
	lastClicked = clicked;
	clicked->applyForce(rp3d::Vector3(-direction.x,-direction.y,-direction.z)*force, worldPoint);
}

void ClickedTrigger::runBox(SystemBase* sys, Entity* ent) {
	clicked->applyForce(rp3d::Vector3(direction.x,direction.y,direction.z)*force, worldPoint);
}