//
// Created by Tim on 16/03/2017.
//

#ifndef BLOCKHOP_CLION_TEXT_H
#define BLOCKHOP_CLION_TEXT_H

#include "texture.h"
#include "shader.h"
#include "components/TextComponent.h"
#include <vector>
#include <glm/glm.hpp>
#include <map>
#include <array>
#include <string>
#include <GL/glew.h>

class Text
{
public:
    Text(Font* inFont);
    ~Text();
    
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

#endif //BLOCKHOP_CLION_TEXT_H
