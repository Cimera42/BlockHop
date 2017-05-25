//
// Created by Tim on 24/05/2017.
//

#include "TextComponent.h"
#include "../ecs/ecsManager.h"
#include "../openGLFunctions.h"
#include <fstream>

COMPONENT_EXPORT(TextComponent, "textComponent")

Font::Font()
{
    std::ifstream i("./textures/OpenSans-Regular.json");
    i >> metrics;
    texture = new Texture("./textures/text.png");
}

Font::~Font()
{
    delete texture;
}

TextComponent::TextComponent()
{
    xBuffer = 0;
    yBuffer = 0;
    createBuffers();
}

TextComponent::~TextComponent()
{
    delete font;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
}

void TextComponent::setValues(json inValues)
{
    //Will throw if incorrect/should automatically be caught by ECSManager
    font = new Font();

    createBuffers();
}

void TextComponent::createBuffers()
{
    glGenBuffers(1, &vertexBuffer);
    createVAO();
}

void TextComponent::createVAO()
{
    glGenVertexArrays(1, &VAO);
    glSetBindVertexArray(VAO);
    //Enable vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    int inc = 0;
    GLuint vertLoc = 0;
    glEnableVertexAttribArray(vertLoc);
    glVertexAttribPointer(vertLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void *) (intptr_t) inc); //attribute, size, type, is normalised?, stride, offset
    inc += sizeof(glm::vec2);

    GLuint uvLoc = 1;
    glEnableVertexAttribArray(uvLoc);
    glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) (intptr_t) inc);
    inc += sizeof(glm::vec2);
    glSetBindVertexArray(0);
}

void TextComponent::fillBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STREAM_DRAW);
}

void TextComponent::set(std::string inText)
{
    text = "";
    vertices.clear();
    xBuffer = 0;
    yBuffer = 0;

    add(inText);
}

void TextComponent::add(std::string inText)
{
    text += inText;
    for(unsigned int i = 0; i < inText.size(); i++)
    {
        json a = font->metrics["chars"][std::string(1, inText.at(i))];
        std::vector<int> metric = a;
        float scale = 1.5f;
        int width = metric[0];
        int height = metric[1];
        int horiAdvance = metric[4];

        if(width != 0 || height != 0)
        {
            width += font->spacing * 2;
            height += font->spacing * 2;

            int horiBearingX = metric[2];
            int horiBearingY = metric[3];
            float w = font->texture->widths[0];
            float h = font->texture->heights[0];
            float posX = metric[5] / w;
            float posY = metric[6] / h;

            vertices.push_back(
                    {glm::vec2(xBuffer + (horiBearingX - font->spacing) * scale, yBuffer - horiBearingY * scale),
                     glm::vec2(posX, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width) * scale,
                                          yBuffer - horiBearingY * scale),
                                glm::vec2(posX + width / w, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing) * scale,
                                          yBuffer + (height - horiBearingY) * scale),
                                glm::vec2(posX, posY + height / h)});

            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width) * scale,
                                          yBuffer - horiBearingY * scale),
                                glm::vec2(posX + width / w, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing) * scale,
                                          yBuffer + (height - horiBearingY) * scale),
                                glm::vec2(posX, posY + height / h)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width) * scale,
                                          yBuffer + (height - horiBearingY) * scale),
                                glm::vec2(posX + width / w, posY + height / h)});
        }

        xBuffer += horiAdvance * scale;
    }

    fillBuffers();
}