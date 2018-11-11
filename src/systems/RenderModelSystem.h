//
// Created by Jono on 11/11/2018.
//

#ifndef BLOCKHOP_RENDERMODELSYSTEM_H
#define BLOCKHOP_RENDERMODELSYSTEM_H

#include "../ecs/system.h"
#include "../shader.h"

class RenderModelSystem : public System<RenderModelSystem>
{
public:
	RenderModelSystem();
	~RenderModelSystem();

	Shader *genericShader;
	Shader *boneShader;

	void update(double dt);
};

#endif //BLOCKHOP_RENDERMODELSYSTEM_H