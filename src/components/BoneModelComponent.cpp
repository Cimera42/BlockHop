//
// Created by Tim on 25/05/2017.
//

#include "BoneModelComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(BoneModelComponent, "boneModelComponent")

BoneModelComponent::BoneModelComponent() {}
BoneModelComponent::~BoneModelComponent() {}

void BoneModelComponent::setValues(json inValues) {
    //Will throw if incorrect/should automatically be caught by ECSManager

}