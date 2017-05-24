//
// Created by Tim on 16/03/2017.
//

#include "text.h"
#include "openGLFunctions.h"
#include "shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

Text::Text(Font* inFont)
{
    font = inFont;
    xBuffer = 0;
    yBuffer = 0;
    
    createBuffers();
}
Text::~Text()
{
    delete font;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
}

void Text::set(std::string inText)
{
    text = "";
    vertices.clear();
    xBuffer = 0;
    yBuffer = 0;
    
    add(inText);
}

void Text::add(std::string inText)
{
    text += inText;
    for(unsigned int i = 0; i < inText.size(); i++)
    {
        std::vector<int> metric = font->metrics["chars"][inText.at(i)];
        
        float scale = 1.5f;
        int width = metric[0];
        int height = metric[1];
        int horiAdvance = metric[4];
        
        if(width != 0 || height != 0)
        {
            width += font->spacing*2;
            height += font->spacing*2;
            
            int horiBearingX = metric[2];
            int horiBearingY = metric[3];
            float w = font->texture->widths[0];
            float h = font->texture->heights[0];
            float posX = metric[5]/w;
            float posY = metric[6]/h;

            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing)*scale, yBuffer - horiBearingY*scale),
                                glm::vec2(posX, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width)*scale, yBuffer - horiBearingY*scale),
                                glm::vec2(posX + width/w, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing)*scale, yBuffer + (height - horiBearingY)*scale),
                                glm::vec2(posX, posY + height/h)});
            
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width)*scale, yBuffer - horiBearingY*scale),
                                glm::vec2(posX + width/w, posY)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing)*scale, yBuffer + (height - horiBearingY)*scale),
                                glm::vec2(posX, posY + height/h)});
            vertices.push_back({glm::vec2(xBuffer + (horiBearingX - font->spacing + width)*scale, yBuffer + (height - horiBearingY)*scale),
                                glm::vec2(posX + width/w, posY + height/h)});
        }
        
        xBuffer += horiAdvance*scale;
    }

    fillBuffers();
}

void Text::createBuffers()
{
    //Generate VUNM (vertexUvNormalMaterial) buffer using data
    glGenBuffers(1, &vertexBuffer);

    createVAO();
}

void Text::fillBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2D), vertices.data(), GL_STREAM_DRAW);
}

void Text::createVAO()
{
    glGenVertexArrays(1, &VAO);
    glSetBindVertexArray(VAO);
        //Enable vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
        int inc = 0;
        GLuint vertLoc = 0;
        glEnableVertexAttribArray(vertLoc);
        glVertexAttribPointer(vertLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) (intptr_t) inc); //attribute, size, type, is normalised?, stride, offset
        inc += sizeof(glm::vec2);
    
        GLuint uvLoc = 1;
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*) (intptr_t) inc);
        inc += sizeof(glm::vec2);
    glSetBindVertexArray(0);
};

void Text::render(Shader* shader)
{
    glm::mat4 p = glm::ortho(-320.0f,320.0f,240.0f,-240.0f);
    glm::mat4 m = glm::translate(glm::vec3(-310.0f,100.0f,0));
    m *= glm::rotate(glm::radians(45.f),glm::vec3(0,0,1));
    
    shader->use();
    glUniformMatrix4fv(shader->getLoc("projMat"), 1, GL_FALSE, &p[0][0]);

    glSetActiveTexture(GL_TEXTURE0);
    glSetBindTexture(GL_TEXTURE_2D_ARRAY, font->texture->textureID);
    glUniform1i(shader->getLoc("textureSampler"), 0);

    glUniformMatrix4fv(shader->getLoc("modelMat"), 1, GL_FALSE, &m[0][0]);
    
    glUniform4f(shader->getLoc("u_colour"), 0.0f,0.0f,0.0f,1.0f);
    glUniform1f(shader->getLoc("u_min"), 0.675f);
    glUniform1f(shader->getLoc("u_max"), 0.775f);

    glSetBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glSetBindVertexArray(0);
}
