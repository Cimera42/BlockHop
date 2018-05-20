//
// Created by Tim on 24/05/2017.
//

#ifndef BLOCKHOP_TEXTCOMPONENT_H
#define BLOCKHOP_TEXTCOMPONENT_H

#include "../loaders/imageAsset.h"
#include "../ecs/component.h"
#include <GL/glew.h>
#include <json.hpp>
#include <glm/vec2.hpp>

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
	~TextComponent() override;
	void setValues(const json &inValues) override;

	Font *getFont() const;
	const std::vector<Vertex2D> &getVertices() const;
	GLuint getVAO() const;

	void set(const std::string &inText);
	void add(const std::string &inText);
	void createBuffers();
	void fillBuffers();
	void createVAO();
};

#endif //BLOCKHOP_TEXTCOMPONENT_H
