//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_CAMERASYSTEM_H
#define BLOCKHOP_CAMERASYSTEM_H

#include "../ecs/system.h"

class CameraSystem : public SystemStatics<CameraSystem>
{
public:
	CameraSystem();
	~CameraSystem();

	void update(double dt);
};

#endif //BLOCKHOP_CAMERASYSTEM_H