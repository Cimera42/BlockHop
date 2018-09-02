#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "scenes/MainGameScene.h"
#include "loaders/assetManager.h"
#include "logger.h"
#include "ecs/ecsManager.h"

Window* window;
bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
	shouldExit = true;
}

int main()
{
	Logger() << "First Line of Program";
	initGLFW();

	window = new Window("Template", 640, 480);
	window->cursorMode(GLFW_CURSOR_DISABLED);

	initGLEW();

	glfwSetWindowCloseCallback(window->glfwWindow, windowCloseEvent);

	//Prime our asset loader
	AssetManager::i()->readConfig();

	//Load scene from file
	MainGameScene* currScene = new MainGameScene();
	currScene->load();

	std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
	start = std::chrono::steady_clock::now();
	previous = start;
	while(!shouldExit)
	{
		current = std::chrono::steady_clock::now();
		std::chrono::duration<double> dt = (current - previous);
		previous = current;

		//Run the current scene
		currScene->run(dt);

		if(glfwGetKey(window->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			shouldExit = true;
		glfwPollEvents();
		glfwSwapBuffers(window->glfwWindow);
	}

	delete ECSManager::i();
	delete AssetManager::i();
	delete window;
	glfwTerminate();
	return 0;
}