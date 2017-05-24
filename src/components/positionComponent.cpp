//
// Created by Tim on 19/04/2017.
//

#include "positionComponent.h"

bool a = ECSManager::i()->exportComponent<PositionComponent>("positionComponent");

PositionComponent::PositionComponent() {}
PositionComponent::~PositionComponent() {}

void PositionComponent::setValues(json inValues) {
    //Will throw if incorrect/should automatically be caught by ECSManager
    position = glm::vec3();
    position.x = inValues["x"].get<float>();
    position.y = inValues["y"].get<float>();
    position.z = inValues["z"].get<float>();
}