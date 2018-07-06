//
// Created by Tim on 24/06/2017.
//

#include "PhysicsSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/TransformComponent.h"
#include "../components/PhysicsComponent.h"
#include "../window.h"
#include "MouseButtonSystem.h"

#include <bullet3/btBulletCollisionCommon.h>

SYSTEM_EXPORT(PhysicsSystem, "physicsSystem")

PhysicsSystem::PhysicsSystem()
{
	accumulator = 0;

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-9.81f,0));
}
PhysicsSystem::~PhysicsSystem()
{
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;
}

void PhysicsSystem::subscribeCallback(Entity *entSubbed)
{
	auto transformComp = entSubbed->getComponent<TransformComponent>("transformComponent");
	auto physicsComp = entSubbed->getComponent<PhysicsComponent>("physicsComponent");
	glm::vec3 startPos = transformComp->getPosition();

	btTransform transformBt;
	transformBt.setIdentity();
	transformBt.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));

	btScalar mass(physicsComp->mass);
	btVector3 localInertia(0,0,0);
	physicsComp->collisionShapeBt->calculateLocalInertia(mass, localInertia);

	btMotionState* motionState = new btDefaultMotionState(transformBt);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
			mass,
			motionState,
			physicsComp->collisionShapeBt,
			localInertia
	);

	btRigidBody* rigidBodyBt = new btRigidBody(rbInfo);
	dynamicsWorld->addRigidBody(rigidBodyBt);

	rigidBodiesBt[entSubbed] = rigidBodyBt;

	for(auto jointData : physicsComp->jointData)
	{
		auto typeName = jointData["type"].get<std::string>();

		auto otherEntityName = jointData.find("entity");
		if(otherEntityName != jointData.end())
		{
			Entity* otherEntity = ECSManager::i()->findEntity((*otherEntityName).get<std::string>());
			if(otherEntity)
			{
				btRigidBody *otherRigidBody = findRigidBody(otherEntity);
				if(typeName == "BallSocket")
				{
					btVector3 pivotThis(
							jointData["pivotThis"]["x"].get<float>(),
							jointData["pivotThis"]["y"].get<float>(),
							jointData["pivotThis"]["z"].get<float>()
					);

					btVector3 pivotThat(
							jointData["pivotThat"]["x"].get<float>(),
							jointData["pivotThat"]["y"].get<float>(),
							jointData["pivotThat"]["z"].get<float>()
					);

					btTypedConstraint *ballSocket = new btPoint2PointConstraint(
						*rigidBodyBt,
						*otherRigidBody,
						pivotThis,
						pivotThat
					);
					dynamicsWorld->addConstraint(ballSocket);
				}
				else if(typeName == "Hinge")
				{
					btVector3 pivotThis(
							jointData["pivotThis"]["x"].get<float>(),
							jointData["pivotThis"]["y"].get<float>(),
							jointData["pivotThis"]["z"].get<float>()
					);

					btVector3 pivotThat(
							jointData["pivotThat"]["x"].get<float>(),
							jointData["pivotThat"]["y"].get<float>(),
							jointData["pivotThat"]["z"].get<float>()
					);

					btVector3 axisThis(
							jointData["axisThis"]["x"].get<float>(),
							jointData["axisThis"]["y"].get<float>(),
							jointData["axisThis"]["z"].get<float>()
					);

					btVector3 axisThat(
							jointData["axisThat"]["x"].get<float>(),
							jointData["axisThat"]["y"].get<float>(),
							jointData["axisThat"]["z"].get<float>()
					);

					btTypedConstraint *hinge = new btHingeConstraint(
						*rigidBodyBt,
						*otherRigidBody,
						pivotThis,
						pivotThat,
						axisThis,
						axisThat
					);
					dynamicsWorld->addConstraint(hinge);
				}
			}
		}
		else
		{
			if(typeName == "BallSocket")
			{
				btVector3 pivot(
					jointData["pivotThis"]["x"].get<float>(),
					jointData["pivotThis"]["y"].get<float>(),
					jointData["pivotThis"]["z"].get<float>()
				);

				btTypedConstraint *ballSocket = new btPoint2PointConstraint(*rigidBodyBt, pivot);
				dynamicsWorld->addConstraint(ballSocket);
			}
			else if(typeName == "Hinge")
			{
				btVector3 pivot(
						jointData["pivotThis"]["x"].get<float>(),
						jointData["pivotThis"]["y"].get<float>(),
						jointData["pivotThis"]["z"].get<float>()
				);

				btVector3 axis(
						jointData["axisThis"]["x"].get<float>(),
						jointData["axisThis"]["y"].get<float>(),
						jointData["axisThis"]["z"].get<float>()
				);

				btTypedConstraint *hinge = new btHingeConstraint(*rigidBodyBt, pivot, axis);
				dynamicsWorld->addConstraint(hinge);
			}
			else if(typeName == "Slider")
			{
				btVector3 pivot(
						jointData["pivotThis"]["x"].get<float>(),
						jointData["pivotThis"]["y"].get<float>(),
						jointData["pivotThis"]["z"].get<float>()
				);

				btVector3 axis(
						jointData["axisThis"]["x"].get<float>(),
						jointData["axisThis"]["y"].get<float>(),
						jointData["axisThis"]["z"].get<float>()
				);

				btVector3 xAxis(1,0,0); // Identity direction
				// Rotate identity direction to desired axis
				btVector3 a = xAxis.cross(axis);
				btScalar w = sqrt(xAxis.length()*xAxis.length() * axis.length()*axis.length()) + xAxis.dot(axis);

				btTransform frameInA = btTransform::getIdentity();
				frameInA.setOrigin(pivot);
				frameInA.setRotation(btQuaternion(a.x(),a.y(),a.z(),w));

//				btTypedConstraint *slider = new btSliderConstraint(
//					*rigidBodyBt,
//					frameInA,
//					true // Dunno what this does...
//				);
				btGeneric6DofConstraint* slider = new btGeneric6DofConstraint(
					*rigidBodyBt,
					frameInA,
					true // Dunno what this does...
				);
				slider->setAngularLowerLimit(btVector3(1.0f,0,0));
				slider->setAngularUpperLimit(btVector3(-1.0f,0,0));
				slider->setLinearLowerLimit(btVector3(0,0,-1.f));
				slider->setLinearUpperLimit(btVector3(0,0,1.f));
				dynamicsWorld->addConstraint(slider);
			}
		}

//		auto otherEntity = ECSManager::i()->findEntity(otherEntityName);
//		if(otherEntity)
//		{
//			auto otherRb = findRigidBody(otherEntity);
//			if(otherRb)
//			{
//				if(typeName == "BallSocket")
//				{
//					rp3d::BallAndSocketJointInfo jointInfo(rigidBody, otherRb, anchorPosition);
//
//					auto collision = jointData.find("collision");
//					if(collision != jointData.end())
//					{
//						jointInfo.isCollisionEnabled = (*collision).get<bool>();
//					}
//
//					rp3d::BallAndSocketJoint* joint;
//					joint = dynamic_cast<rp3d::BallAndSocketJoint*>(dynamicsWorld->createJoint(jointInfo));
//					physicsComp->joints.emplace_back(joint);
//				}
//				else if(typeName == "Fixed")
//				{
//					rp3d::FixedJointInfo jointInfo(rigidBody, otherRb, anchorPosition);
//
//					auto collision = jointData.find("collision");
//					if(collision != jointData.end())
//					{
//						jointInfo.isCollisionEnabled = (*collision).get<bool>();
//					}
//
//					rp3d::FixedJoint* joint;
//					joint = dynamic_cast<rp3d::FixedJoint*>(dynamicsWorld->createJoint(jointInfo));
//					physicsComp->joints.emplace_back(joint);
//				}
//				else if(typeName == "Slider")
//				{
//					rp3d::Vector3 axis;
//					axis.x = jointData["axis"]["x"].get<float>();
//					axis.y = jointData["axis"]["y"].get<float>();
//					axis.z = jointData["axis"]["z"].get<float>();
//
//					rp3d::SliderJointInfo jointInfo(rigidBody, otherRb, anchorPosition ,axis);
//
//					auto limits = jointData.find("limits");
//					if(limits != jointData.end())
//					{
//						jointInfo.isLimitEnabled = true;
//						jointInfo.minTranslationLimit = (*limits)[0].get<float>();
//						jointInfo.maxTranslationLimit = (*limits)[1].get<float>();
//					}
//
//					auto motorData = jointData.find("motor");
//					if(motorData != jointData.end())
//					{
//						jointInfo.isMotorEnabled = true;
//						jointInfo.motorSpeed = (*motorData)["speed"].get<float>();
//						jointInfo.maxMotorForce = (*motorData)["maxForce"].get<float>();
//					}
//
//					auto collision = jointData.find("collision");
//					if(collision != jointData.end())
//					{
//						jointInfo.isCollisionEnabled = (*collision).get<bool>();
//					}
//
//					rp3d::SliderJoint* joint;
//					joint = dynamic_cast<rp3d::SliderJoint*>(dynamicsWorld->createJoint(jointInfo));
//					physicsComp->joints.emplace_back(joint);
//				}
//				else if(typeName == "Hinge")
//				{
//					rp3d::Vector3 axis;
//					axis.x = jointData["axis"]["x"].get<float>();
//					axis.y = jointData["axis"]["y"].get<float>();
//					axis.z = jointData["axis"]["z"].get<float>();
//
//					rp3d::HingeJointInfo jointInfo(rigidBody, otherRb, anchorPosition, axis);
//					auto limits = jointData.find("limits");
//					if(limits != jointData.end())
//					{
//						jointInfo.isLimitEnabled = true;
//						jointInfo.minAngleLimit = (*limits)[0].get<float>();
//						jointInfo.maxAngleLimit = (*limits)[1].get<float>();
//					}
//
//					auto motorData = jointData.find("motor");
//					if(motorData != jointData.end())
//					{
//						jointInfo.isMotorEnabled = true;
//						jointInfo.motorSpeed = (*motorData)["speed"].get<float>();
//						jointInfo.maxMotorTorque = (*motorData)["maxTorque"].get<float>();
//					}
//
//					auto collision = jointData.find("collision");
//					if(collision != jointData.end())
//					{
//						jointInfo.isCollisionEnabled = (*collision).get<bool>();
//					}
//
//					rp3d::HingeJoint* joint;
//					joint = dynamic_cast<rp3d::HingeJoint*>(dynamicsWorld->createJoint(jointInfo));
//					physicsComp->joints.emplace_back(joint);
//				}
//			}
//		}
	}
}
//
//// Class WorldRaycastCallback
//class MyCallbackClass : public rp3d::RaycastCallback {
//
//public:
//	glm::vec3 dir;
//	explicit MyCallbackClass(glm::vec3 inDir) {
//		dir = inDir;
//	}
//
//	rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override
//	{
//		if(info.body->getType() == rp3d::DYNAMIC)
//		{
//			auto rb = dynamic_cast<reactphysics3d::RigidBody*>(info.body);
//			rb->applyForce(rp3d::Vector3(dir.x,dir.y,dir.z)*20.0f, info.worldPoint);
//		}
//
//		// Return a fraction of 1.0 to gather all hits
//		return rp3d::decimal(1.0);
//	}
//};

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
		glm::vec3 endGlm = startGlm + direction*50.0f;

		btVector3 directionBt(direction.x, direction.y, direction.z);
		btVector3 startBt(startGlm.x, startGlm.y, startGlm.z);
		btVector3 endBt(endGlm.x, endGlm.y, endGlm.z);
		btCollisionWorld::ClosestRayResultCallback closestResult(startBt, endBt);

		dynamicsWorld->rayTest(startBt, endBt, closestResult);

		if(closestResult.hasHit())
		{
			btRigidBody *rigidBody = (btRigidBody *) btRigidBody::upcast(closestResult.m_collisionObject);
			rigidBody->activate();
			btVector3 worldHit = closestResult.m_hitPointWorld;
			btVector3 localHit = rigidBody->getCenterOfMassTransform().inverse()*worldHit;
			Logger() << "W " << worldHit.x() << " - " << worldHit.y() << " - " << worldHit.z();
			Logger() << "L " << localHit.x() << " - " << localHit.y() << " - " << localHit.z();
			rigidBody->applyForce(directionBt * 10.0f * b, localHit);

			// Get Entity "PickPos"
			// Set position worldHit
		}
	}

	while(accumulator >= idealTimestep)
	{
		dynamicsWorld->stepSimulation(idealTimestep, 10);

		accumulator -= idealTimestep;
	}

	for(auto entity : getEntities())
	{
		auto transformComp = entity->getComponent<TransformComponent>("transformComponent");

		btRigidBody* rb = findRigidBody(entity);
		if(rb)
		{
			btTransform transform;
			rb->getMotionState()->getWorldTransform(transform);
			btVector3 pos = transform.getOrigin();
			btQuaternion rot = transform.getRotation();
			transformComp->setPosition(glm::vec3(pos.getX(), pos.getY(), pos.getZ()));
			transformComp->setRotation(glm::quat(rot.getW(), rot.getX(), rot.getY(), rot.getZ()));

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

btRigidBody* PhysicsSystem::findRigidBody(Entity* toFind)
{
	auto t = rigidBodiesBt.find(toFind);
	if(t != rigidBodiesBt.end())
		return t->second;
	return nullptr;
}
