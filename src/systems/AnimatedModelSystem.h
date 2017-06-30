//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_ANIMATEDMODELSYSTEM_H
#define BLOCKHOP_ANIMATEDMODELSYSTEM_H

#include "../ecs/system.h"
#include "../shader.h"

class AnimatedModelSystem : public System 
{
	static bool exported;
public:
	AnimatedModelSystem();
	~AnimatedModelSystem();

	Shader *genericShader;
	Shader *boneShader;
	
	void update(double dt);
};

#endif //BLOCKHOP_ANIMATEDMODELSYSTEM_H