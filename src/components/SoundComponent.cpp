//
// Created by Tim on 2/09/2018.
//

#include "SoundComponent.h"
#include "../ecs/ecsManager.h"

COMPONENT_EXPORT(SoundComponent, "soundComponent")

SoundComponent::SoundComponent() {}
SoundComponent::~SoundComponent() {}

void SoundComponent::setValues(json inValues)
{
	filename = inValues["filename"];

	SoLoud::result loadResult = wav.load(filename.c_str());
	if(loadResult != SoLoud::SO_NO_ERROR)
		throw std::invalid_argument("Could not find audio file \"" + filename + "\"");
}