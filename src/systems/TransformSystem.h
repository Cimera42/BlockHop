//
// Created by Tim on 17/11/2018.
//

#ifndef BLOCKHOP_TRANSFORMSYSTEM_H
#define BLOCKHOP_TRANSFORMSYSTEM_H

#include "../ecs/system.h"

class TransformSystem : public System<TransformSystem>
{
public:
	TransformSystem();
	~TransformSystem();

	void update(double dt);
};

#endif //BLOCKHOP_TRANSFORMSYSTEM_H