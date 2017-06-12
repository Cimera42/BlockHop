#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "ecs/ecsManager.h"
#include "ecs/ecsLoader.h"
#define GLM_ENABLE_EXPERIMENTAL

Window* window;
bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
	shouldExit = true;
}

int main()
{
	/*glm::mat4 a = glm::mat4(0.0f,0.0f,-1.0f,0.0f,
							0.0f,1.0f,0.0f,0.0f,
							1.0f,0.0f,0.0f,0.0f,
							0.0f,0.0f,0.0f,1.0f);

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(a, scale, rotation, position, skew, perspective);
	Logger(1) << "Bone 1: ";
	Logger(1) << "  Matrix: " << a;
	Logger(1) << "  Position: " << position;
	Logger(1) << "  Rotation: " << rotation;
	Logger(1) << "  Scale: " << scale;

	glm::vec3 scale2;
	glm::quat rotation2;
	glm::vec3 position2;
	glm::vec3 skew2;
	glm::vec4 perspective2;

	glm::mat4 b = glm::mat4(0.0f,1.0f,0.0f,0.0f,
							0.0f,0.0f,1.0f,0.0f,
							1.0f,0.0f,0.0f,0.0f,
							-1.0f,1.0f,0.0f,1.0f);
	
	glm::decompose(b, scale2, rotation2, position2, skew2, perspective2);
	Logger(1) << "Bone 2: ";
	Logger(1) << "  Matrix: " << b;
	Logger(1) << "  Position: " << position2;
	Logger(1) << "  Rotation: " << rotation2;
	Logger(1) << "  Scale: " << scale2;

	return 0;*/
	Logger(1) << "First Line of Program";
	initGLFW();

	window = new Window("Template", 640, 480);
	window->cursorMode(GLFW_CURSOR_DISABLED);

	initGLEW();

	glfwSetWindowCloseCallback(window->glfwWindow, windowCloseEvent);

	//Load scene from file
	ECSLoader loader = ECSLoader();
	loader.readStream("testModel.json");

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