//
// Created by Tim on 25/05/2017.
//

#ifndef BLOCKHOP_TEXTSYSTEM_H
#define BLOCKHOP_TEXTSYSTEM_H

#include "../ecs/system.h"
#include "../shader.h"
#include "../components/TextComponent.h"
#include "../components/TransformComponent.h"

class TextSystem : public System<TextSystem>
{
public:
	TextSystem();
	~TextSystem();

	Shader *textShader;

	void update(double dt, double alpha);

	void renderText(TransformComponent *transform, TextComponent *text);
};

#endif //BLOCKHOP_TEXTSYSTEM_H
