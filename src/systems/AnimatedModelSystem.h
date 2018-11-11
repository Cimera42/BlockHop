//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_ANIMATEDMODELSYSTEM_H
#define BLOCKHOP_ANIMATEDMODELSYSTEM_H

#include "../ecs/system.h"

class AnimatedModelSystem : public System<AnimatedModelSystem>
{
public:
	AnimatedModelSystem();
	~AnimatedModelSystem();

	void update(double dt);
};

#endif //BLOCKHOP_ANIMATEDMODELSYSTEM_H