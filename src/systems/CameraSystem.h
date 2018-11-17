//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_CAMERASYSTEM_H
#define BLOCKHOP_CAMERASYSTEM_H

#include "../ecs/system.h"

class CameraSystem : public System<CameraSystem>
{
public:
	CameraSystem();
	~CameraSystem();

	void update(double dt, double alpha);
};

#endif //BLOCKHOP_CAMERASYSTEM_H