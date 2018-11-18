#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "scenes/MainGameScene.h"
#include "loaders/configAsset.h"
#include "loaders/assetManager.h"
#include "gameSettings.h"
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

	//Prime our asset loader and load engines main config
	AssetManager::get().readConfig();
	GameSettings::get().loadSettings();

	//Logger() << clientConfig->config << std::endl;

	// Initialise window, opengl
	initGLFW();

	window = new Window(
		GameSettings::get().getWindowName().c_str(),
		GameSettings::get().getWindowWidth(),
		GameSettings::get().getWindowHeight(),
		GameSettings::get().getWindowFullscreen(),
		GameSettings::get().getVSync());
	window->cursorMode(GLFW_CURSOR_DISABLED);

	initGLEW();

	glfwSetWindowCloseCallback(window->glfwWindow, windowCloseEvent);

	//Load default assets for engine
	AssetManager::get().loadDefault();

	//Load scene from file
	MainGameScene* currScene = new MainGameScene();
	currScene->load();

	//Have fun in our game loop
	std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
	start = std::chrono::steady_clock::now();
	previous = start;
	double accumulator; //TODO precision?
	while(!shouldExit)
	{
		window->updateViewport();

		current = std::chrono::steady_clock::now();
		std::chrono::duration<double> timeDiff = (current - previous);
		double frameTime = timeDiff.count();
		if(frameTime > GameSettings::get().maxFrameTime) {
			frameTime = GameSettings::get().maxFrameTime;
		}
		previous = current;

		accumulator += frameTime;

		//Run the current scene's logic
		while(accumulator >= GameSettings::get().updateTimestep)
		{
			currScene->runLogic(GameSettings::get().updateTimestep, 0);
			accumulator -= GameSettings::get().updateTimestep;
		}

		const double alpha = accumulator / GameSettings::get().updateTimestep;

		//Run the current scene's presentation and make use of extrapolation
		currScene->runPresentation(frameTime, alpha * GameSettings::get().updateTimestep);

		if(glfwGetKey(window->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			shouldExit = true;

		//TODO - DEMO ONLY - REMOVE ONCE MENU - PREVENT MULTIPLE PRESSES ETC
		if(glfwGetKey(window->glfwWindow, GLFW_KEY_8) == GLFW_PRESS) {
			window->resize(1080, 720, true);
		}
		if(glfwGetKey(window->glfwWindow, GLFW_KEY_7) == GLFW_PRESS) {
			window->resize(640, 480, false);
		}

		glfwPollEvents();
		glfwSwapBuffers(window->glfwWindow);
	}

	delete window;
	glfwTerminate();
	return 0;
}