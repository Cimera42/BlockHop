#include <iostream>
#include "openGLFunctions.h"
#include "boneModel.h"
#include "camera.h"
#include "window.h"
#include "text.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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

    Shader* genericShader = new Shader("shaders/basic3d/3dvert.vert", "shaders/basic3d/3dfrag.frag");
    genericShader->addLoc("modelMat");
    genericShader->addLoc("viewMat");
    genericShader->addLoc("projMat");
    genericShader->addLoc("textureSampler");

    Shader* boneShader = new Shader("shaders/bone3d/boneVert.vert", "shaders/bone3d/boneFrag.frag", "shaders/flat.geom");
    boneShader->addLoc("modelMat");
    boneShader->addLoc("viewMat");
    boneShader->addLoc("projMat");
    boneShader->addLoc("textureSampler");
    boneShader->addLoc("boneMats");
    
    Shader* textShader = new Shader("shaders/text/textVert.vert", "shaders/text/textFrag.frag");
    textShader->addLoc("modelMat");
    textShader->addLoc("projMat");
    textShader->addLoc("textureSampler");
    textShader->addLoc("u_colour");
    textShader->addLoc("u_min");
    textShader->addLoc("u_max");
    
    Font* f = new Font();
    Text* text = new Text(f);
    text->add("ABCDEFG");

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

        model.renderModel(genericShader, boneShader, camera);
        modelBone.renderModel(genericShader, boneShader, camera);
        
        text->set("#" + std::to_string(glfwGetTime()));
        text->render(textShader);

        if(glfwGetKey(window.glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            shouldExit = true;
        glfwPollEvents();
        glfwSwapBuffers(window.glfwWindow);
    }
    
    delete text;
    window.destroy();
    glfwTerminate();
}
