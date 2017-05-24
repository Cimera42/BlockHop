//
// Created by Jono on 14/03/2017.
//

#include "testSystem.h"
#include "../components/positionComponent.h"
#include "../components/TextComponent.h"
#include "../shader.h"

bool System::exported = ECSManager::i()->exportSystem<TestSystem>("testSystem");

Shader *textShader;
TestSystem::TestSystem() {

    textShader = new Shader("shaders/text/textVert.vert", "shaders/text/textFrag.frag");
    textShader->addLoc("modelMat");
    textShader->addLoc("projMat");
    textShader->addLoc("textureSampler");
    textShader->addLoc("u_colour");
    textShader->addLoc("u_min");
    textShader->addLoc("u_max");
}
TestSystem::~TestSystem() {}

void TestSystem::update(double dt) {
    //Logger(1)<<"Virtual inheritance test works!";
    for(auto entity : getEntities())
    {
        PositionComponent* pos = entity->getComponent<PositionComponent>("positionComponent");
        TextComponent* text = entity->getComponent<TextComponent>("TextComponent");

        Logger(1) << *pos;
        text->set(std::to_string(dt));
        text->render(textShader);
    }
}