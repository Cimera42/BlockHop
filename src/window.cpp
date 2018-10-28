#include "window.h"
#include "gameSettings.h"

Window::Window(){};
Window::Window(const char* inTitle, int inWidth, int inHeight, bool inFullscreen)
{
	title = inTitle;
	width = inWidth;
	height = inHeight;
	isFullscreen = inFullscreen;

	createGLFWWindow();
	resize(width, height, isFullscreen);
}

Window::~Window()
{
	if(glfwWindow)
		destroy();
}

void Window::destroy()
{
	glfwDestroyWindow(glfwWindow);
	glfwWindow = nullptr;
}

void Window::createGLFWWindow()
{
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindow = glfwCreateWindow(width,height,title,nullptr,nullptr);
	glfwMakeContextCurrent(glfwWindow);

	//glfwSetWindowUserPointer(glfwWindow, this);
	//glfwSetWindowSizeCallback(glfwWindow, Window::resizeCallback );
	glfwSwapInterval(1);
}

void Window::updateViewport() {
	if ( _updateViewport )
	{
		// Update opengl
		int f1, f2;
		glfwGetFramebufferSize( glfwWindow, &f1, &f2 );
		glViewport( 0, 0, f1, f2 );

		// Commit to saving new settings to file
		GameSettings::get().setWindow(title,width,height,isFullscreen);
		GameSettings::get().saveSettings();

		_updateViewport = false;
	}
};

void Window::resize(int inWidth, int inHeight, bool inFullscreen) {
	//if not fullscreen- place window in center of screen
	width = inWidth;
	height = inHeight;
	isFullscreen = inFullscreen;
	if(!isFullscreen) {
		const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int window_width = mode->width;
		int window_height = mode->height;
		int posX = (window_width-width)/2;
		int posY = (window_height-height)/2;
		glfwSetWindowMonitor(glfwWindow, NULL, posX, posY, width, height, GLFW_DONT_CARE);
	} else {
		glfwSetWindowMonitor(glfwWindow, glfwGetPrimaryMonitor(), 0, 0, width, height, GLFW_DONT_CARE);
	}
	_updateViewport = true;
}

void Window::cursorMode(int mode)
{
	glfwSetInputMode(glfwWindow, GLFW_CURSOR, mode);
}

