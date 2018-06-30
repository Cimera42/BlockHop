//
// Created by Tim on 24/06/2017.
//

#include "PhysicsSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../window.h"
#include "MouseButtonSystem.h"


SYSTEM_EXPORT(PhysicsSystem, "physicsSystem")

PhysicsSystem::PhysicsSystem()
{
	accumulator = 0;

	rp3d::Vector3 gravity(0,-9.81f,0);
	dynamicsWorld = new rp3d::DynamicsWorld(gravity);
	//dynamicsWorld->enableSleeping(false);
}
PhysicsSystem::~PhysicsSystem()
{
//	dynamicsWorld.destroyCollisionBody(collisionBody);
	delete dynamicsWorld;
}

void PhysicsSystem::subscribeCallback(Entity *entSubbed)
{
	auto transformComp = entSubbed->getComponent<TransformComponent>("transformComponent");
	auto physicsComp = entSubbed->getComponent<PhysicsComponent>("physicsComponent");
	glm::vec3 startPos = transformComp->getPosition();

	rp3d::Vector3 initPos(startPos.x,startPos.y,startPos.z);
	rp3d::Quaternion initRot = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPos, initRot);
	rp3d::RigidBody* rigidBody;

	rigidBody = dynamicsWorld->createRigidBody(transform);
	rigidBodies[entSubbed] = rigidBody;

	rp3d::ProxyShape* proxyShape = rigidBody->addCollisionShape(physicsComp->getCollisionShape(), rp3d::Transform::identity(), 1.0f);
	physicsComp->setCollisionShapeInstance(proxyShape);
	rigidBody->setType(physicsComp->getMode());

	for(auto jointData : physicsComp->jointData)
	{
		auto otherEntityName = jointData["entity"].get<std::string>();
		auto typeName = jointData["type"].get<std::string>();

		rp3d::Vector3 anchorPosition;
		anchorPosition.x = jointData["anchor"]["x"].get<float>();
		anchorPosition.y = jointData["anchor"]["y"].get<float>();
		anchorPosition.z = jointData["anchor"]["z"].get<float>();

		auto otherEntity = ECSManager::i()->findEntity(otherEntityName);
		if(otherEntity)
		{
			auto physicsSystem = ECSManager::i()->findSystem<PhysicsSystem>("physicsSystem");
			auto otherRb = physicsSystem->findRigidBody(otherEntity);
			if(otherRb)
			{
				if(typeName == "BallSocket")
				{
					rp3d::BallAndSocketJointInfo jointInfo(rigidBody, otherRb, anchorPosition);

					auto collision = jointData.find("collision");
					if(collision != jointData.end())
					{
						jointInfo.isCollisionEnabled = (*collision).get<bool>();
					}

					rp3d::BallAndSocketJoint* joint;
					joint = dynamic_cast<rp3d::BallAndSocketJoint*>(dynamicsWorld->createJoint(jointInfo));
					physicsComp->joints.emplace_back(joint);
				}
				else if(typeName == "Fixed")
				{
					rp3d::FixedJointInfo jointInfo(rigidBody, otherRb, anchorPosition);

					auto collision = jointData.find("collision");
					if(collision != jointData.end())
					{
						jointInfo.isCollisionEnabled = (*collision).get<bool>();
					}

					rp3d::FixedJoint* joint;
					joint = dynamic_cast<rp3d::FixedJoint*>(dynamicsWorld->createJoint(jointInfo));
					physicsComp->joints.emplace_back(joint);
				}
				else if(typeName == "Slider")
				{
					rp3d::Vector3 axis;
					axis.x = jointData["axis"]["x"].get<float>();
					axis.y = jointData["axis"]["y"].get<float>();
					axis.z = jointData["axis"]["z"].get<float>();

					rp3d::SliderJointInfo jointInfo(rigidBody, otherRb, anchorPosition ,axis);

					auto limits = jointData.find("limits");
					if(limits != jointData.end())
					{
						jointInfo.isLimitEnabled = true;
						jointInfo.minTranslationLimit = (*limits)[0].get<float>();
						jointInfo.maxTranslationLimit = (*limits)[1].get<float>();
					}

					auto motorData = jointData.find("motor");
					if(motorData != jointData.end())
					{
						jointInfo.isMotorEnabled = true;
						jointInfo.motorSpeed = (*motorData)["speed"].get<float>();
						jointInfo.maxMotorForce = (*motorData)["maxForce"].get<float>();
					}

					auto collision = jointData.find("collision");
					if(collision != jointData.end())
					{
						jointInfo.isCollisionEnabled = (*collision).get<bool>();
					}

					rp3d::SliderJoint* joint;
					joint = dynamic_cast<rp3d::SliderJoint*>(dynamicsWorld->createJoint(jointInfo));
					physicsComp->joints.emplace_back(joint);
				}
				else if(typeName == "Hinge")
				{
					rp3d::Vector3 axis;
					axis.x = jointData["axis"]["x"].get<float>();
					axis.y = jointData["axis"]["y"].get<float>();
					axis.z = jointData["axis"]["z"].get<float>();

					rp3d::HingeJointInfo jointInfo(rigidBody, otherRb, anchorPosition, axis);
					auto limits = jointData.find("limits");
					if(limits != jointData.end())
					{
						jointInfo.isLimitEnabled = true;
						jointInfo.minAngleLimit = (*limits)[0].get<float>();
						jointInfo.maxAngleLimit = (*limits)[1].get<float>();
					}

					auto motorData = jointData.find("motor");
					if(motorData != jointData.end())
					{
						jointInfo.isMotorEnabled = true;
						jointInfo.motorSpeed = (*motorData)["speed"].get<float>();
						jointInfo.maxMotorTorque = (*motorData)["maxTorque"].get<float>();
					}

					auto collision = jointData.find("collision");
					if(collision != jointData.end())
					{
						jointInfo.isCollisionEnabled = (*collision).get<bool>();
					}

					rp3d::HingeJoint* joint;
					joint = dynamic_cast<rp3d::HingeJoint*>(dynamicsWorld->createJoint(jointInfo));
					physicsComp->joints.emplace_back(joint);
				}
			}
		}
	}
}

// Class WorldRaycastCallback
class MyCallbackClass : public rp3d::RaycastCallback {

public:
	glm::vec3 dir;
	explicit MyCallbackClass(glm::vec3 inDir) {
		dir = inDir;
	}

	rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override
	{
		if(info.body->getType() == rp3d::DYNAMIC)
		{
			auto rb = dynamic_cast<reactphysics3d::RigidBody*>(info.body);
			rb->applyForce(rp3d::Vector3(dir.x,dir.y,dir.z)*20.0f, info.worldPoint);
		}

		// Return a fraction of 1.0 to gather all hits
		return rp3d::decimal(1.0);
	}
};

extern Window window;
void PhysicsSystem::update(double dt)
{
	accumulator += dt;

	auto mouseButtonSystem = ECSManager::i()->findSystem<MouseButtonSystem>("mouseButtonSystem");
	int b = 0;
	if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		b = 1;
	}
	else if(mouseButtonSystem->isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		b = -1;
	}
	if(b != 0)
	{
		Entity* cameraEntity = ECSManager::i()->findEntity("Camera");
		auto cameraTransform = cameraEntity->getComponent<TransformComponent>("transformComponent");
		glm::vec3 startGlm = cameraTransform->getPosition();
		glm::vec3 direction = cameraTransform->getForward();
		glm::vec3 mDirection = direction*20.0f;

		rp3d::Vector3 start(startGlm.x,startGlm.y,startGlm.z);
		rp3d::Vector3 end = start + rp3d::Vector3(mDirection.x,mDirection.y,mDirection.z);
		rp3d::Ray ray(start,end);

		MyCallbackClass callback(direction * (float) b);
		dynamicsWorld->raycast(ray, &callback);
	}

	while(accumulator >= idealTimestep)
	{
		dynamicsWorld->update(idealTimestep);

		accumulator -= idealTimestep;
	}

	for(auto entity : getEntities())
	{
		auto transformComp = entity->getComponent<TransformComponent>("transformComponent");

		rp3d::RigidBody* rb = findRigidBody(entity);
		if(rb)
		{
			if(rb->getType() == rp3d::DYNAMIC)
			{
				rp3d::Vector3 pos = rb->getTransform().getPosition();
				rp3d::Quaternion rot = rb->getTransform().getOrientation();
				transformComp->setPosition(glm::vec3(pos.x, pos.y, pos.z));
				transformComp->setRotation(glm::quat(rot.w, rot.x, rot.y, rot.z));

//				auto contactListElement = rb->getContactManifoldsList();
//				if(contactListElement && b != 0)
//				{
//					auto norm = contactListElement->contactManifold->getAverageContactNormal() * -200 * b;
//					auto point = contactListElement->contactManifold->getContactPoint(0)->getWorldPointOnBody1();
//					rb->applyForce(norm, point);
//				}
			}
		}
	}
}

rp3d::RigidBody* PhysicsSystem::findRigidBody(Entity* toFind)
{
	auto t = rigidBodies.find(toFind);
	if(t != rigidBodies.end())
		return t->second;
	return nullptr;
}
