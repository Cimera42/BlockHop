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
    textShader = new Shader("./assets/shaders/text/textVert.vert", "./assets/shaders/text/textFrag.frag");
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

        //Default data setting for now, just set to delta time
        text->set(std::to_string(dt));
        renderText(transform, text);
    }
}

void TextSystem::renderText(TransformComponent *transform, TextComponent *text)
{
    glm::mat4 p = glm::ortho(-320.0f, 320.0f, 240.0f, -240.0f);
    glm::mat4 m = transform->getMatrix();

    textShader->use();
    glUniformMatrix4fv(textShader->getLoc("projMat"), 1, GL_FALSE, &p[0][0]);

    //Bind texture atlas
    glSetActiveTexture(GL_TEXTURE0);
    glSetBindTexture(GL_TEXTURE_2D_ARRAY, text->getFont()->texture);
    glUniform1i(textShader->getLoc("textureSampler"), 0);

    glUniformMatrix4fv(textShader->getLoc("modelMat"), 1, GL_FALSE, &m[0][0]);

    glUniform4f(textShader->getLoc("u_colour"), 0.0f, 0.0f, 0.0f, 1.0f);
    //Signed Distance Field text rendering config
    //TODO move this somewhere configurable
    glUniform1f(textShader->getLoc("u_min"), 0.675f);
    glUniform1f(textShader->getLoc("u_max"), 0.775f);

    glSetBindVertexArray(text->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, (GLint) text->getVertices().size());
    glSetBindVertexArray(0);
}