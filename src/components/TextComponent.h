//
// Created by Tim on 24/05/2017.
//

#ifndef BLOCKHOP_TEXTCOMPONENT_H
#define BLOCKHOP_TEXTCOMPONENT_H

#include <array>
#include <map>
#include <json.hpp>
#include "../loaders/imageAsset.h"
#include "../shader.h"
#include "../ecs/component.h"
#include <glm/glm.hpp>

using json = nlohmann::json;

class Font
{
public:
	Font();
	~Font();

	ImageAsset* image;
	GLuint texture;
	int spacing = 3;
	json metrics;
};

struct Vertex2D
{
	glm::vec2 pos;
	glm::vec2 uv;
};

class TextComponent : public Component
{
	static bool exported;

	Font* font;
	std::vector<Vertex2D> vertices;

	GLuint VAO = 0;
	GLuint vertexBuffer;

	std::string text;
	int xBuffer;
	int yBuffer;

public:
	TextComponent();
	~TextComponent();
	void setValues(json inValues);

	Font *getFont() const;
	const std::vector<Vertex2D> &getVertices() const;
	GLuint getVAO() const;

	void set(std::string inText);
	void add(std::string inText);
	void createBuffers();
	void fillBuffers();
	void createVAO();
};


#endif //BLOCKHOP_TEXTCOMPONENT_H
