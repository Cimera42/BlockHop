//
// Created by Tim on 12/09/2019.
//

#ifndef BLOCKHOP_MARCHINGCUBERENDERSYSTEM_H
#define BLOCKHOP_MARCHINGCUBERENDERSYSTEM_H

#include "../ecs/system.h"
#include "../shader.h"

class MarchingCubeRenderSystem : public System<MarchingCubeRenderSystem>
{
public:
	MarchingCubeRenderSystem();
	~MarchingCubeRenderSystem();

	Shader *marchingCubeShader;

	void update(double dt);
};

#endif //BLOCKHOP_MARCHINGCUBERENDERSYSTEM_H