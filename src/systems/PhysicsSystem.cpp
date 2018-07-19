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

btQuaternion fromVecToVecQuat(btVector3 from, btVector3 to)
{
	// Rotate direction to desired direction
	float vecDot = from.dot(to);
	if(vecDot < -0.99999) // Check if from and to are opposite
	{
		// 180 Degree rotation on any axis (x in this case)
		return btQuaternion(1,0,0,0);
	}
	else if(vecDot < 0.99999) // Check if from and to are the same
	{
		Logger() << vecDot;
		btVector3 crossVec = from.cross(to);
		btScalar w = sqrt(from.length()*from.length() * to.length()*to.length()) + vecDot;
		return btQuaternion(crossVec.x(),crossVec.y(),crossVec.z(),w);
	}
	// No rotation
	return btQuaternion(0,0,0,1);
}

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
	glm::quat rotation = transformComp->getRotation();

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
	transform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));

	btScalar mass(physicsComp->mass);
	btVector3 localInertia(0,0,0);
	physicsComp->collisionShape->calculateLocalInertia(mass, localInertia);

	btMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
			mass,
			motionState,
			physicsComp->collisionShape,
			localInertia
	);

	auto rigidBody = new btRigidBody(rbInfo);
	dynamicsWorld->addRigidBody(rigidBody);

	rigidBodies[entSubbed] = rigidBody;

	for(auto jointData : physicsComp->jointData)
	{
		std::string typeName = jointData["type"];

		Entity* otherEntity = nullptr;
		btRigidBody *otherRigidBody = nullptr;
		auto otherEntityName = jointData.find("entity");
		if(otherEntityName != jointData.end())
		{
			otherEntity = ECSManager::i()->findEntity(*otherEntityName);
			otherRigidBody = findRigidBody(otherEntity);
		}

		btVector3 pivotThis = jointData["pivotThis"];

		btVector3 axisThis;
		if(typeName == "Hinge" || typeName == "Slider" || typeName == "Generic")
		{
			axisThis = jointData["axisThis"];
		}

		btVector3 pivotThat;
		btVector3 axisThat;
		if(otherRigidBody)
		{
			pivotThat = jointData["pivotThat"];

			if(typeName == "Hinge" || typeName == "Slider" || typeName == "Generic")
			{
				axisThat = jointData["axisThat"];
			}
		}

		btTransform frameInA = btTransform::getIdentity();
		btTransform frameInB = btTransform::getIdentity();
		if(typeName == "Slider" || typeName == "Generic")
		{
			frameInA.setOrigin(pivotThis);
			frameInA.setRotation(fromVecToVecQuat(btVector3(1, 0, 0), axisThis));

			if(otherRigidBody)
			{
				frameInB.setOrigin(pivotThat);
				frameInB.setRotation(fromVecToVecQuat(btVector3(1, 0, 0), axisThat));
			}
		}

		if(typeName == "BallSocket")
		{
			btPoint2PointConstraint *ballSocket;
			if(otherRigidBody)
			{
				ballSocket = new btPoint2PointConstraint(
					*otherRigidBody,
					*rigidBody,
					pivotThat,
					pivotThis
				);
			}
			else
			{
				ballSocket = new btPoint2PointConstraint(*rigidBody, pivotThis);
			}
			new btPoint2PointConstraint{*rigidBody, pivotThis};
			dynamicsWorld->addConstraint(ballSocket);
		}
		else if(typeName == "Hinge")
		{
			btHingeConstraint *hinge;
			if(otherRigidBody)
			{
				hinge = new btHingeConstraint(
					*otherRigidBody,
					*rigidBody,
					pivotThat,
					pivotThis,
					axisThat,
					axisThis
				);
			}
			else
			{
				hinge = new btHingeConstraint(*rigidBody, pivotThis, axisThis);
			}
			dynamicsWorld->addConstraint(hinge);
		}
		else if(typeName == "Generic" || typeName == "Slider")
		{
			btGeneric6DofConstraint *genericJoint;
			if(otherRigidBody)
			{
				genericJoint = new btGeneric6DofConstraint(
					*otherRigidBody,
					*rigidBody,
					frameInB,
					frameInA,
					true // Dunno what this does...
				);
			}
			else
			{
				genericJoint = new btGeneric6DofConstraint(
					*rigidBody,
					frameInA,
					true // Dunno what this does...
				);
			}

			if(typeName == "Slider")
			{
				genericJoint->setAngularLowerLimit(btVector3(0, 0, 0));
				genericJoint->setAngularUpperLimit(btVector3(0, 0, 0));

				//Lower > Upper = free movement
				float lowerLimit = 1.0f;
				float upperLimit = -1.0f;
				auto limits = jointData.find("limits");
				if(limits != jointData.end())
				{
					//Lower == Upper = locked
					lowerLimit = (*limits)[0];
					upperLimit = (*limits)[1];
					if(lowerLimit >= upperLimit)
						throw std::invalid_argument(entSubbed->getName() + " > " + physicsComp->getName() +
													" > Slider Joint > 'Lower limit greater than upper limit.'");
				}
				genericJoint->setLinearLowerLimit(btVector3(lowerLimit, 0, 0));
				genericJoint->setLinearUpperLimit(btVector3(upperLimit, 0, 0));

				auto motorData = jointData.find("motor");
				if(motorData != jointData.end())
				{
					auto translationMotor = genericJoint->getTranslationalLimitMotor();
					translationMotor->m_enableMotor[0] = true;
					translationMotor->m_targetVelocity = btVector3((*motorData)["speed"], 0, 0);
					translationMotor->m_maxMotorForce = btVector3((*motorData)["maxForce"], 0, 0);
				}
			}
			else
			{
				auto doAxisLimit = [](json data, btVector3 *min, btVector3 *max, std::string key, int index) {
					auto axisData = data.find(key);
					if(axisData != data.end())
					{
						min->m_floats[index] = (*axisData)[0];
						max->m_floats[index] = (*axisData)[1];
					}
				};

				auto limits = jointData.find("limits");
				if(limits != jointData.end())
				{
					btVector3 angularLimitMin(0, 0, 0);
					btVector3 angularLimitMax(0, 0, 0);
					auto angular = (*limits).find("angular");
					if(angular != (*limits).end())
					{
						auto doAngularLimit = [doAxisLimit, angular, &angularLimitMin, &angularLimitMax]
											   (std::string key,int index) {
							doAxisLimit((*angular), &angularLimitMin, &angularLimitMax, key, index);
						};

						doAngularLimit("x", 0);
						doAngularLimit("y", 1);
						doAngularLimit("z", 2);

					}
					genericJoint->setAngularLowerLimit(angularLimitMin);
					genericJoint->setAngularUpperLimit(angularLimitMax);

					btVector3 linearLimitMin(0, 0, 0);
					btVector3 linearLimitMax(0, 0, 0);
					auto linear = (*limits).find("linear");
					if(linear != (*limits).end())
					{
						auto doLinearLimit = [doAxisLimit, linear, &linearLimitMin, &linearLimitMax]
											  (std::string key, int index) {
							doAxisLimit((*linear), &linearLimitMin, &linearLimitMax, key, index);
						};

						doLinearLimit("x", 0);
						doLinearLimit("y", 1);
						doLinearLimit("z", 2);
					}
					genericJoint->setLinearLowerLimit(linearLimitMin);
					genericJoint->setLinearUpperLimit(linearLimitMax);
				}

				auto motorData = jointData.find("motor");
				if(motorData != jointData.end())
				{
					auto translationMotor = genericJoint->getTranslationalLimitMotor();

					btVector3 velocity(0, 0, 0);
					btVector3 maxForce(0, 0, 0);

					auto doMotor = [motorData, translationMotor, &velocity, &maxForce](std::string key, int index) {
						auto axisData = (*motorData).find(key);
						if(axisData != (*motorData).end())
						{
							translationMotor->m_enableMotor[index] = true;
							velocity.m_floats[index] = (*axisData)["speed"];
							maxForce.m_floats[index] = (*axisData)["maxForce"];
						}
					};

					doMotor("x", 0);
					doMotor("y", 1);
					doMotor("z", 2);

					translationMotor->m_targetVelocity = btVector3(velocity);
					translationMotor->m_maxMotorForce = btVector3(maxForce);
				}
			}

			dynamicsWorld->addConstraint(genericJoint);
		}
	}
}

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
		glm::vec3 directionGlm = cameraTransform->getForward();
		glm::vec3 endGlm = startGlm + directionGlm*50.0f;

		btVector3 directionBt(directionGlm.x, directionGlm.y, directionGlm.z);
		btVector3 startBt(startGlm.x, startGlm.y, startGlm.z);
		btVector3 endBt(endGlm.x, endGlm.y, endGlm.z);
		btCollisionWorld::ClosestRayResultCallback closestResult(startBt, endBt);

		dynamicsWorld->rayTest(startBt, endBt, closestResult);

		if(closestResult.hasHit())
		{
			auto rigidBody = (btRigidBody *) btRigidBody::upcast(closestResult.m_collisionObject);
			rigidBody->activate();
			btVector3 worldHit = closestResult.m_hitPointWorld;
			btVector3 localHit = worldHit - rigidBody->getCenterOfMassTransform().getOrigin();
			rigidBody->applyForce(directionBt * 10.0f * b, localHit);

			// Get Entity "PickPos"
			// Set position worldHit
			auto indicator = ECSManager::i()->findEntity("RaycastIndicator");
			auto indicatorTransform = indicator->getComponent<TransformComponent>("transformComponent");
			indicatorTransform->setPosition(glm::vec3(worldHit.x(),worldHit.y(),worldHit.z()));
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
		}
	}
}

btRigidBody* PhysicsSystem::findRigidBody(Entity* toFind)
{
	auto t = rigidBodies.find(toFind);
	if(t != rigidBodies.end())
		return t->second;
	return nullptr;
}
