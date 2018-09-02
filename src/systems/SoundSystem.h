//
// Created by Tim on 2/09/2018.
//

#ifndef BLOCKHOP_SOUNDSYSTEM_H
#define BLOCKHOP_SOUNDSYSTEM_H

#include "../ecs/system.h"

#include <soloud/soloud.h>

class SoundSystem : public System<SoundSystem>
{
public:
	SoundSystem();
	~SoundSystem();

	SoLoud::Soloud soloud;

	void subscribeCallback(Entity* entSubbed) override;
	void update(double dt);
};

#endif //BLOCKHOP_SOUNDSYSTEM_H