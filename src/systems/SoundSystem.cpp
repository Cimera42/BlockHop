//
// Created by Tim on 2/09/2018.
//

#include "SoundSystem.h"
#include "../ecs/ecsManager.h"
#include "../components/SoundComponent.h"
#include "../components/TransformComponent.h"
#include <soloud/soloud_audiosource.h>

SYSTEM_EXPORT(SoundSystem, "soundSystem")

SoundSystem::SoundSystem()
{
	soloud.init();
}
SoundSystem::~SoundSystem()
{
	soloud.deinit();
}

void SoundSystem::subscribeCallback(Entity *entSubbed)
{
	auto soundComponent = entSubbed->getComponent<SoundComponent>();
	auto transformComponent = entSubbed->getComponent<TransformComponent>();
	auto pos = transformComponent->getPosition();
	soundComponent->soundHandle = soloud.play3d(
		soundComponent->wav,
		pos.x,
		pos.y,
		pos.z
	);
	soloud.set3dSourceAttenuation(soundComponent->soundHandle, SoLoud::AudioSource::ATTENUATION_MODELS::INVERSE_DISTANCE, 0.5);

//	soloud.setVolume(handle1, 0.5f);            // Set volume; 1.0f is "normal"
//	soloud.setPan(handle1, -0.2f);              // Set pan; -1 is left, 1 is right
//	soloud.setRelativePlaySpeed(handle1, 1.5f); // Play a bit slower; 1.0f is normal
}

void SoundSystem::update(double dt)
{
	updateSystemTriggers();

	Entity* cameraEntity = ECSManager::i()->findEntity("Camera");
	auto cameraTransform = cameraEntity->getComponent<TransformComponent>();
	auto pos = cameraTransform->getPosition();
	auto forward = cameraTransform->getForward();
	auto up = cameraTransform->getUp();
	soloud.set3dListenerParameters(
		pos.x,
		pos.y,
		pos.z,
		forward.x,
		forward.y,
		forward.z,
		up.x,
		up.y,
		up.z
	);
	soloud.update3dAudio();

	for(auto entity : getEntities())
	{
		updateEntityTriggers(entity);

		//XComponent* x = entity->getComponent<XComponent>();
	}
}