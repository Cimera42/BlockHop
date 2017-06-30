#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "ecs/ecsManager.h"
#include "ecs/ecsLoader.h"
#include "loaders/assetManager.h"

Window* window;
bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
	shouldExit = true;
}

int main()
{
	Logger(1) << "First Line of Program";
	initGLFW();

	window = new Window("Template", 640, 480);
	window->cursorMode(GLFW_CURSOR_DISABLED);

	initGLEW();

	glfwSetWindowCloseCallback(window->glfwWindow, windowCloseEvent);

	//Prime our asset loader
	AssetManager::i()->readConfig();

	//Load scene from file
	ECSLoader ecsLoader = ECSLoader();
	ecsLoader.readStream("testModel.json");

	std::chrono::time_point<std::chrono::steady_clock> start, previous, current;
	start = std::chrono::steady_clock::now();
	previous = start;
	while(!shouldExit)
	{
		glClearColor(0.55f, 0.65f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		current = std::chrono::steady_clock::now();
		std::chrono::duration<double> dt = (current - previous);
		previous = current;

		//Run update on our systems
		for(auto sysPair : ECSManager::i()->gameSystems)
		{
			auto system = sysPair.second;
			//Get time difference for updating systems

			//Call both basic update and one with timestep, implementation dependant
			system->update(dt.count());

			//Total clock duration
			std::chrono::duration<double> totalTime = current - start;
			//Logger()<<dt.count()<<" "<<totalTime.count();
		}

		if(glfwGetKey(window->glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			shouldExit = true;
		glfwPollEvents();
		glfwSwapBuffers(window->glfwWindow);
	}

	delete window;
	glfwTerminate();
	return 0;
}