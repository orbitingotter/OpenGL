#include "Window.h"

#include <iostream>
#include <Vendor/stb_image/stb_image.h>

Window::Window(const std::string& title, int width, int height)
	: mTitle(title), mWidth(width), mHeight(height)
{
	if (!glfwInit())
	{
		std::cout << "ERROR : GLFW was not initlialized" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		std::cout << "ERROR : GLFW window could not be created" << std::endl;
	}

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	fprintf(stdout, "GLEW : Using GLEW %s\n", glewGetString(GLEW_VERSION));
	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::IsRunning() const
{
	return !glfwWindowShouldClose(mWindow);
}

void Window::Close() const
{
	glfwSetWindowShouldClose(mWindow, 1);
}

void Window::SetVSync(bool vsync) const
{
	if (vsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

const std::string& Window::GetTitle() const
{
	return mTitle;
}

void Window::SetTitle(const std::string& title)
{
	mTitle = title;
	glfwSetWindowTitle(mWindow, title.c_str());
}

void Window::SetSize(int x, int y)
{
	mWidth = x;
	mHeight = y;
	glfwSetWindowSize(mWindow, x, y);
}

int Window::GetWidth() const
{
	return mWidth;
}

int Window::GetHeight() const
{
	return mHeight;
}

GLFWwindow* Window::Get() const
{
	return mWindow;
}

void Window::OnUpdate() const
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}
