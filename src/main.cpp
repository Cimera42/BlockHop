#include <iostream>
#include "openGLFunctions.h"
#include "boneModel.h"
#include "camera.h"
#include "window.h"
#include "ecs/ecsManager.h"
#include "ecs/testComponent.h"
#include "ecs/testSystem.h"
#include "ecs/ecsLoader.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define TEST_MAIN 1

glm::vec2 lastPos;
glm::vec3 worldPos(0,0,0);
Camera camera;

bool shouldExit = false;
void windowCloseEvent(GLFWwindow* closingWindow)
{
    shouldExit = true;
}
void mouseMoveEvent(GLFWwindow* window, double xpos, double ypos)
{
    if(lastPos.x == 0 && lastPos.y == 0)
        lastPos = glm::vec2(xpos,ypos);
    camera.pitch += (lastPos.y-ypos)/100.0f;
    camera.yaw += (lastPos.x-xpos)/100.0f;

    lastPos = glm::vec2(xpos,ypos);
}

#ifdef TEST_MAIN
int main() {

    ECSLoader loader = ECSLoader();
    loader.readStream("testjson.json");


    /*std::vector<std::string> ttt;
    ttt.push_back("testComponent");
    TestSystem* f = static_cast<TestSystem*> (ECSManager::i()->createSystem("testSystem", ttt));
    Logger(1)<< "Test value: "<<f->ok;

    json j1 = {{"test", true}};
    TestComponent* e = static_cast<TestComponent*> (ECSManager::i()->createComponent("testComponent", j1));
    Logger(1)<< "Test value: "<<e->testInt;


    //Entity creation
    std::vector<json> vvv;
    vvv.push_back(j1);
    std::vector<std::string> bbb;
    bbb.push_back("testComponent");
    Entity* newEnt = ECSManager::i()->createEntity("Ent", bbb, vvv);

    //Testing
    //TODO Create a test
    for(auto ent : f->getEntities()) {
        Logger(1)<<"These should match: "<<ent<<" "<<newEnt;
    }

    newEnt->removeComponent("testComponent");
    Logger(1)<<"System should have no subbed entities. Entity size: "<<f->getEntities().size();

    newEnt->addComponent("testComponent", e);
    Logger(1)<<"System should have 1 subbed entity. Entity size: "<<f->getEntities().size();

    f->update();

    Entity* confirm = ECSManager::i()->findEntity("Ent");
    Logger(1)<<"These should also match: "<<newEnt<<" "<<confirm;

    System* searchSys = ECSManager::i()->findSystem("testSystem");
    Logger(1)<<"So should these: "<<searchSys<<" "<<f;

    Logger(1)<<"And this should be null: "<<ECSManager::i()->findEntity("ent");

    TestComponent* getComp = static_cast<TestComponent*> (newEnt->getComponent("testComponent"));
    Logger(1)<<"This should show test value again: "<<getComp->testInt;

    Logger(1)<<"And this should be null once again: "<<newEnt->getComponent("testSystem");*/
}
#else
int main()
{
    Logger(1) << "First Line of Program";
    initGLFW();

    Window window("Template", 640, 480);
    window.cursorMode(GLFW_CURSOR_DISABLED);

    initGLEW();

    glfwSetWindowCloseCallback(window.glfwWindow, windowCloseEvent);
    glfwSetCursorPosCallback(window.glfwWindow, mouseMoveEvent);

    BoneModel model("models/TestBonePlane/TestBonePlane.fbx");
    model.load();

    BoneModel modelBone("models/Org/org3.fbx");
    modelBone.load();

    camera.link(&worldPos);
    camera.update();
    camera.perspective(90.0f, (float)window.width/window.height, 0.001f, 100.0f);

    Shader genericShader("shaders/basic3d/3dvert.vert", "shaders/basic3d/3dfrag.frag");
    genericShader.addLoc("modelMat");
    genericShader.addLoc("viewMat");
    genericShader.addLoc("projMat");
    genericShader.addLoc("textureSampler");

    Shader boneShader("shaders/bone3d/boneVert.vert", "shaders/bone3d/boneFrag.frag", "shaders/flat.geom");
    boneShader.addLoc("modelMat");
    boneShader.addLoc("viewMat");
    boneShader.addLoc("projMat");
    boneShader.addLoc("textureSampler");
    boneShader.addLoc("boneMats");

    float delta = 0;
    float lastFrame = (float) glfwGetTime();

    while(!shouldExit)
    {
        delta = ((float) glfwGetTime() - lastFrame);
        lastFrame = (float) glfwGetTime();

        glClearColor(0.55f,0.65f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window.glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
            worldPos += camera.forward*delta*5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
            worldPos -= camera.forward*delta*5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
            worldPos -= camera.right*delta*5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
            worldPos += camera.right*delta*5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            worldPos += glm::vec3(0,1,0)*delta*5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            worldPos -= glm::vec3(0,1,0)*delta*5.0f;

        camera.update();

        for(unsigned int i = 0; i < model.parts.size(); i++)
        {
            Part part = model.parts[i];
            glm::mat4 modelMatrix = glm::translate(part.position);
            modelMatrix = glm::scale(modelMatrix, part.scale);
            modelMatrix = modelMatrix * glm::toMat4(part.rotation);

            if(model.normalMeshes.find(part.mesh) != model.normalMeshes.end())
            {
                genericShader.use();
                glUniformMatrix4fv(genericShader.getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
                glUniformMatrix4fv(genericShader.getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_2D_ARRAY, model.texture.textureID);
                glUniform1i(genericShader.getLoc("textureSampler"), 0);

                glUniformMatrix4fv(genericShader.getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

                glSetBindVertexArray(model.normalMeshes[part.mesh].VAO);
                glDrawElements(GL_TRIANGLES, model.normalMeshes[part.mesh].indices.size(), GL_UNSIGNED_INT, 0);
                glSetBindVertexArray(0);
            }
            else
            {
                boneShader.use();
                glUniformMatrix4fv(boneShader.getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
                glUniformMatrix4fv(boneShader.getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_2D_ARRAY, model.texture.textureID);
                glUniform1i(boneShader.getLoc("textureSampler"), 0);

                glUniformMatrix4fv(boneShader.getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

                if (model.boneMeshes[part.mesh].boneMats.size())
                {
                    glUniformMatrix4fv(boneShader.getLoc("boneMats"), model.boneMeshes[part.mesh].boneMats.size(),
                                       GL_FALSE, &model.boneMeshes[part.mesh].boneMats.data()[0][0][0]);
                }

                glSetBindVertexArray(model.boneMeshes[part.mesh].VAO);
                glDrawElements(GL_TRIANGLES, model.boneMeshes[part.mesh].indices.size(), GL_UNSIGNED_INT, 0);
                glSetBindVertexArray(0);
            }
        }

        for(unsigned int i = 0; i < modelBone.parts.size(); i++)
        {
            Part part = modelBone.parts[i];
            glm::mat4 modelMatrix = glm::translate(part.position);
            modelMatrix = glm::scale(modelMatrix, part.scale);
            modelMatrix = modelMatrix * glm::toMat4(part.rotation);

            if(modelBone.normalMeshes.find(part.mesh) != modelBone.normalMeshes.end())
            {
                genericShader.use();
                glUniformMatrix4fv(genericShader.getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
                glUniformMatrix4fv(genericShader.getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_2D_ARRAY, modelBone.texture.textureID);
                glUniform1i(genericShader.getLoc("textureSampler"), 0);

                glUniformMatrix4fv(genericShader.getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

                glSetBindVertexArray(modelBone.normalMeshes[part.mesh].VAO);
                glDrawElements(GL_TRIANGLES, modelBone.normalMeshes[part.mesh].indices.size(), GL_UNSIGNED_INT, 0);
                glSetBindVertexArray(0);
            }
            else
            {
                boneShader.use();
                glUniformMatrix4fv(boneShader.getLoc("viewMat"), 1, GL_FALSE, &camera.viewMatrix[0][0]);
                glUniformMatrix4fv(boneShader.getLoc("projMat"), 1, GL_FALSE, &camera.projectionMatrix[0][0]);

                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_2D_ARRAY, modelBone.texture.textureID);
                glUniform1i(boneShader.getLoc("textureSampler"), 0);

                glUniformMatrix4fv(boneShader.getLoc("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);

                if (modelBone.boneMeshes[part.mesh].boneMats.size())
                {
                    glUniformMatrix4fv(boneShader.getLoc("boneMats"), modelBone.boneMeshes[part.mesh].boneMats.size(),
                                       GL_FALSE, &modelBone.boneMeshes[part.mesh].boneMats.data()[0][0][0]);
                }

                glSetBindVertexArray(modelBone.boneMeshes[part.mesh].VAO);
                glDrawElements(GL_TRIANGLES, modelBone.boneMeshes[part.mesh].indices.size(), GL_UNSIGNED_INT, 0);
                glSetBindVertexArray(0);
            }
        }

        if(glfwGetKey(window.glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            shouldExit = true;
        glfwPollEvents();
        glfwSwapBuffers(window.glfwWindow);
    }

    window.destroy();
    glfwTerminate();
}
#endif