//
// Created by Tim on 25/05/2017.
//

#include "TextSystem.h"
#include "../openGLFunctions.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

SYSTEM_EXPORT(TextSystem, "textSystem")

TextSystem::TextSystem()
{
    textShader = new Shader("shaders/text/textVert.vert", "shaders/text/textFrag.frag");
    textShader->addLoc("modelMat");
    textShader->addLoc("projMat");
    textShader->addLoc("textureSampler");
    textShader->addLoc("u_colour");
    textShader->addLoc("u_min");
    textShader->addLoc("u_max");
}

TextSystem::~TextSystem()
{
    delete textShader;
}

void TextSystem::update(double dt)
{
    for(auto entity : getEntities())
    {
        TransformComponent* transform = entity->getComponent<TransformComponent>("transformComponent");
        TextComponent* text = entity->getComponent<TextComponent>("textComponent");

        text->set(std::to_string(dt));
        renderText(transform, text);
    }
}

void TextSystem::renderText(TransformComponent *trans, TextComponent *text)
{
    glm::mat4 p = glm::ortho(-320.0f, 320.0f, 240.0f, -240.0f);
    glm::mat4 m = glm::mat4();
    m *= glm::translate(trans->position);
    m *= glm::toMat4(trans->rotation);
    m *= glm::scale(trans->scale);

    textShader->use();
    glUniformMatrix4fv(textShader->getLoc("projMat"), 1, GL_FALSE, &p[0][0]);

    glSetActiveTexture(GL_TEXTURE0);
    glSetBindTexture(GL_TEXTURE_2D_ARRAY, text->font->texture->textureID);
    glUniform1i(textShader->getLoc("textureSampler"), 0);

    glUniformMatrix4fv(textShader->getLoc("modelMat"), 1, GL_FALSE, &m[0][0]);

    glUniform4f(textShader->getLoc("u_colour"), 0.0f, 0.0f, 0.0f, 1.0f);
    glUniform1f(textShader->getLoc("u_min"), 0.675f);
    glUniform1f(textShader->getLoc("u_max"), 0.775f);

    glSetBindVertexArray(text->VAO);
    glDrawArrays(GL_TRIANGLES, 0, text->vertices.size());
    glSetBindVertexArray(0);
}