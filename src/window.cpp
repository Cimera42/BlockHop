#include "window.h"

Window::Window(const char* inTitle, int inWidth, int inHeight)
{
    title = inTitle;
    width = inWidth;
    height = inHeight;

    createGLFWWindow();
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
    glfwWindow = glfwCreateWindow(width,height,title,nullptr,nullptr);
    glfwMakeContextCurrent(glfwWindow);
}

void Window::cursorMode(int mode)
{
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, mode);
}

