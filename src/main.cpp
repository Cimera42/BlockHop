#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "ecs/ecsManager.h"
#include "ecs/ecsLoader.h"
#include "camera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#define TEST_MAIN 0

glm::vec2 lastPos;
glm::vec3 worldPos(0, 0, 0);
Camera camera;

bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
    shouldExit = true;
}

void mouseMoveEvent(GLFWwindow *window, double xpos, double ypos)
{
    if(lastPos.x == 0 && lastPos.y == 0)
        lastPos = glm::vec2(xpos, ypos);
    camera.pitch += (lastPos.y - ypos) / 100.0f;
    camera.yaw += (lastPos.x - xpos) / 100.0f;

    lastPos = glm::vec2(xpos, ypos);
}

#if TEST_MAIN
int main() {

    ECSLoader loader = ECSLoader();
    loader.readStream("testjson.json");


    std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
    start = std::chrono::steady_clock::now();
    previous = start;
    bool exitFlag = false;
    while(!exitFlag) {
        //Run update on our systems! Woohoo
        for (auto sysPair : ECSManager::i()->gameSystems) {
            auto system = sysPair.second;
            //Get time difference for updating systems
            current = std::chrono::steady_clock::now();
            std::chrono::duration<double> dt = (current - previous);
            previous = current;

            //Call both basic update and one with timestep, implementation dependant
            system->update(dt.count());

            //Total clock duration
            std::chrono::duration<double> totalTime = current - start;
            //Logger()<<dt.count()<<" "<<totalTime.count();

            if(totalTime.count() > 3) //Only run for 3 seconds for now!
                exitFlag = true;
        }
    }
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

    //Load scene from file
    ECSLoader loader = ECSLoader();
    loader.readStream("testModel.json");
    
    camera.link(&worldPos);
    camera.update();
    camera.perspective(90.0f, (float) window.width / window.height, 0.001f, 100.0f);

    std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
    start = std::chrono::steady_clock::now();
    previous = start;
    while(!shouldExit)
    {
        glClearColor(0.55f, 0.65f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Run update on our systems
        for(auto sysPair : ECSManager::i()->gameSystems)
        {
            auto system = sysPair.second;
            //Get time difference for updating systems
            current = std::chrono::steady_clock::now();
            std::chrono::duration<double> dt = (current - previous);
            previous = current;
			
			float dt_f = (float) dt.count();
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
				worldPos += camera.forward * dt_f * 5.0f;
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
				worldPos -= camera.forward * dt_f * 5.0f;
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
				worldPos -= camera.right * dt_f * 5.0f;
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
				worldPos += camera.right * dt_f * 5.0f;
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
				worldPos += glm::vec3(0, 1, 0) * dt_f * 5.0f;
			if(glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				worldPos -= glm::vec3(0, 1, 0) * dt_f * 5.0f;

			camera.update();

            //Call both basic update and one with timestep, implementation dependant
            system->update(dt.count());

            //Total clock duration
            std::chrono::duration<double> totalTime = current - start;
            //Logger()<<dt.count()<<" "<<totalTime.count();
        }

        if(glfwGetKey(window.glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            shouldExit = true;
        glfwPollEvents();
        glfwSwapBuffers(window.glfwWindow);
    }
/*
    BoneModel model("models/TestBonePlane/TestBonePlane.fbx");
    model.load();

    BoneModel modelBone("models/Org/org3.fbx");
    modelBone.load();


    float delta = 0;
    float lastFrame = (float) glfwGetTime();

    while(!shouldExit)
    {
        delta = ((float) glfwGetTime() - lastFrame);
        lastFrame = (float) glfwGetTime();

        glClearColor(0.55f, 0.65f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window.glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
            worldPos += camera.forward * delta * 5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
            worldPos -= camera.forward * delta * 5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
            worldPos -= camera.right * delta * 5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
            worldPos += camera.right * delta * 5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            worldPos += glm::vec3(0, 1, 0) * delta * 5.0f;
        if(glfwGetKey(window.glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            worldPos -= glm::vec3(0, 1, 0) * delta * 5.0f;

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

    delete text;*/
    window.destroy();
    glfwTerminate();
}

#endif