//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_MOUSEINPUTSYSTEM_H
#define BLOCKHOP_MOUSEINPUTSYSTEM_H

#include <glm/glm.hpp>
#include "../ecs/system.h"

class MouseInputSystem : public System<MouseInputSystem>
{
	glm::vec2 lastPos;

public:
	MouseInputSystem();
	~MouseInputSystem();

	glm::vec2 getLastPos() const;
	void setLastPos(glm::vec2 inLastPos);

	void update(double dt, double alpha);
};

#endif //BLOCKHOP_MOUSEINPUTSYSTEM_H