#include <iostream>
#include "openGLFunctions.h"
#include "window.h"
#include "scenes/MainGameScene.h"
#include "loaders/assetManager.h"
#include "logger.h"

Window* window;
bool shouldExit = false;

void windowCloseEvent(GLFWwindow *closingWindow)
{
	shouldExit = true;
}

struct Holder
{
	virtual std::string getname() = 0;
};

template <typename T>
struct Base: public Holder
{
	static const std::string name;
	std::string getname() { return name; }
};

struct A : public Base<A>
{
	A(int innum) : num(innum) {
	}
	int num;
};
template<> const std::string Base<A>::name = "a";

struct B : public Base<B>
{
};
template<> const std::string Base<B>::name = "b";

std::vector<Holder*> vec;

int main()
{
	Logger() << "First Line of Program";

	vec.emplace_back(new A(24));
	vec.emplace_back(new B());

	for(auto d : vec)
		Logger() << d->getname();

	auto a = dynamic_cast<A*>(vec[0]);
	Logger() << a->num;


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

	delete window;
	glfwTerminate();
	return 0;
}