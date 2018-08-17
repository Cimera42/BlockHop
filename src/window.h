#ifndef WINDOW_H_INCLUDED
#define WINDOW_H_INCLUDED

#include <GLFW/glfw3.h>
class Window
{
	bool _updateViewport = false;
	bool _isFullscreen = false;

public:
	Window();
	Window(const char* title, int width, int height);
	~Window();

	GLFWwindow* glfwWindow;
	int width, height;
	const char* title;

	void updateViewport();
	void destroy();
	void createGLFWWindow();
	void cursorMode(int mode);
	void resize(int width, int height, bool fullscreen);
	//static void resizeCallback(GLFWwindow* window, int x, int y);
};

#endif // WINDOW_H_INCLUDED
