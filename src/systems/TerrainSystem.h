//
// Created by Tim on 16/08/2018.
//

#ifndef BLOCKHOP_TERRAINSYSTEM_H
#define BLOCKHOP_TERRAINSYSTEM_H

#include "../ecs/system.h"
#include "../shader.h"

class TerrainSystem : public System<TerrainSystem>
{
public:
	TerrainSystem();
	~TerrainSystem();

	Shader *terrainShader;

	void update(double dt);
};

#endif //BLOCKHOP_TERRAINSYSTEM_H