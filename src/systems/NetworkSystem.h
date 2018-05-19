//
// Created by Tim on 19/05/2018.
//

#ifndef BLOCKHOP_NETWORKSYSTEM_H
#define BLOCKHOP_NETWORKSYSTEM_H

#include "../ecs/system.h"

class NetworkSystem : public System
{
	static bool exported;
public:
	NetworkSystem();
	~NetworkSystem();

	void update(double dt);
};


#endif //BLOCKHOP_NETWORKSYSTEM_H
