//
// Created by Tim on 24/05/2017.
//

#ifndef BLOCKHOP_CLION_TEXTCOMPONENT_H
#define BLOCKHOP_CLION_TEXTCOMPONENT_H

#include <array>
#include <map>
#include <json.hpp>
#include "../texture.h"
#include "../shader.h"
#include "../ecs/component.h"
#include <glm/glm.hpp>

// for convenience
using json = nlohmann::json;

class Font
{
public:
    Font();
    ~Font();

    Texture* texture;
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
public:
    TextComponent();
    ~TextComponent();
    void setValues(json inValues);
	static bool exported;

    Font* font;
    std::vector<Vertex2D> vertices;

    GLuint VAO = 0;
    GLuint vertexBuffer;

    std::string text;
    int xBuffer;
    int yBuffer;

    void set(std::string inText);
    void add(std::string inText);
    void createBuffers();
    void fillBuffers();
    void createVAO();
    void render(Shader* shader);

};


#endif //BLOCKHOP_CLION_TEXTCOMPONENT_H
