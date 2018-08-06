//
// Created by Tim on 24/06/2017.
//

#include "PhysicsComponent.h"
#include "../ecs/ecsManager.h"
#include "../mesh.h"
#include "../loaders/modelAsset.h"
#include "../loaders/assetManager.h"

#include <bullet3/btBulletCollisionCommon.h>
#include <bullet3/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

COMPONENT_EXPORT(PhysicsComponent, "physicsComponent")

PhysicsComponent::PhysicsComponent() {}
PhysicsComponent::~PhysicsComponent() {}

btVector3 glmToBt(glm::vec3 in)
{
	return btVector3(in.x,in.y,in.z);
}

void PhysicsComponent::setValues(json inValues)
{
	jsonData = inValues;

	principalTransform.setIdentity();
	collisionShape = loadShape(inValues);

	mass = inValues["mass"];
}

btCollisionShape* PhysicsComponent::loadShape(json inValues)
{
	std::string shapeName = inValues["colliderShape"];

	// Primitives
	if(shapeName == "cube")
	{
		return new btBoxShape(inValues["halfDimensions"].get<btVector3>());
	}
	else if(shapeName == "sphere")
	{
		return new btSphereShape(inValues["radius"].get<float>()); // Requires .get for some reason
	}
	else if(shapeName == "capsule")
	{
		return new btCapsuleShape(inValues["radius"], inValues["height"]);
	}
	else if(shapeName == "cylinder")
	{
		return new btCylinderShape(btVector3(inValues["radius"], inValues["halfHeight"], 0));
	}
	else if(shapeName == "cone")
	{
		return new btConeShape(inValues["radius"], inValues["height"]);
	}
	else if(shapeName == "convex")
	{
		auto typedCollisionShape = new btConvexHullShape();

		auto points = inValues.find("points");
		if(points != inValues.end())
		{
			std::vector<json> pointData = (*points);
			for(const auto &point : pointData)
			{
				// Don't recalc AABB while adding points
				typedCollisionShape->addPoint(btVector3(point), false);
			}
			typedCollisionShape->recalcLocalAabb();
		}
		else
		{
			auto model = dynamic_cast<ModelAsset*>(AssetManager::i()->loadSync(inValues["mesh"]));
			auto mesh = model->normalMeshes[model->meshParts[0]->mesh];
			for(auto index : mesh->indices)
			{
				typedCollisionShape->addPoint(glmToBt(mesh->vertices[index]));
			}
		}

		return typedCollisionShape;
	}
	else if(shapeName == "compound")
	{
		auto typedCollisionShape = new btCompoundShape();

		std::vector<json> shapes = inValues["shapes"];
		std::vector<btScalar> masses;
		for(const auto &shapeData : shapes)
		{
			btTransform transform(shapeData["rotation"].get<btQuaternion>(), shapeData["position"]);

			typedCollisionShape->addChildShape(transform, loadShape(shapeData));
			masses.emplace_back(1);
		}

		btVector3 inertia;
		typedCollisionShape->calculatePrincipalAxisTransform(masses.data(), principalTransform, inertia);

		const int countChildren = typedCollisionShape->getNumChildShapes();
		for(int i = 0; i < countChildren; i++)
		{
			btTransform newTransform = principalTransform.inverse() * typedCollisionShape->getChildTransform(i);

			typedCollisionShape->updateChildTransform(i, newTransform, i == countChildren-1);
		}

		return typedCollisionShape;
	}

//	btHeightfieldTerrainShape
//	mass = 0
}