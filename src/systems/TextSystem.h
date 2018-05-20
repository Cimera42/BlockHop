//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_TEXTSYSTEM_H
#define BLOCKHOP_TEXTSYSTEM_H

#include "../ecs/system.h"
#include "../components/TransformComponent.h"
#include "../components/TextComponent.h"
#include "../shader.h"

class TextSystem : public System
{
	static bool exported;
public:
	TextSystem();
	~TextSystem();

	Shader *textShader;

	void update(double dt);

	void renderText(TransformComponent *transform, TextComponent *text);
};

#endif //BLOCKHOP_TEXTSYSTEM_H
