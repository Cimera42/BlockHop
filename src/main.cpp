#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "scenes/MainGameScene.h"
#include "loaders/configAsset.h"
#include "loaders/assetManager.h"
#include "logger.h"

Window* window;
bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
	shouldExit = true;
}

int main()
{
	Logger() << "First Line of Program";

	//Prime our asset loader and load engines main config
	AssetManager::i()->readConfig();
	ConfigAsset* clientConfig = static_cast<ConfigAsset*>(AssetManager::i()->loadSync("./conf/clientConfig.conf"));

	//Logger() << clientConfig->config << std::endl;

	// Initialise window, opengl
	initGLFW();

	window = new Window("Template", 640, 480);
	window->cursorMode(GLFW_CURSOR_DISABLED);

	initGLEW();

	glfwSetWindowCloseCallback(window->glfwWindow, windowCloseEvent);

	//Load default assets for engine
	AssetManager::i()->loadDefault();

	//Load scene from file
	MainGameScene* currScene = new MainGameScene();
	currScene->load();

	//Have fun in our game loop
	std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
	start = std::chrono::steady_clock::now();
	previous = start;
	while(!shouldExit)
	{
		window->updateViewport();

		current = std::chrono::steady_clock::now();
		std::chrono::duration<double> dt = (current - previous);
		previous = current;

		//Run the current scene
		currScene->run(dt);

		if(glfwGetKey(window->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			shouldExit = true;

		if(glfwGetKey(window->glfwWindow, GLFW_KEY_8) == GLFW_PRESS) {
			window->resize(1080, 720, true);
		}

		glfwPollEvents();
		glfwSwapBuffers(window->glfwWindow);
	}

	delete window;
	delete AssetManager::i();
	glfwTerminate();
	return 0;
}