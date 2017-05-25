//
// Created by Tim on 19/04/2017.
//

#include "TransformComponent.h"

COMPONENT_EXPORT(TransformComponent, "transformComponent")

TransformComponent::TransformComponent() {}
TransformComponent::~TransformComponent() {}

void TransformComponent::setValues(json inValues) {
    //Will throw if incorrect/should automatically be caught by ECSManager
    position.x = inValues["position"]["x"].get<float>();
    position.y = inValues["position"]["y"].get<float>();
    position.z = inValues["position"]["z"].get<float>();

    rotation.w = inValues["rotation"]["w"].get<float>();
    rotation.x = inValues["rotation"]["x"].get<float>();
    rotation.y = inValues["rotation"]["y"].get<float>();
    rotation.z = inValues["rotation"]["z"].get<float>();

    scale.x = inValues["scale"]["x"].get<float>();
    scale.y = inValues["scale"]["y"].get<float>();
    scale.z = inValues["scale"]["z"].get<float>();
}