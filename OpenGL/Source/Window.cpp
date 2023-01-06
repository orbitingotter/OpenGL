#include "Window.h"

#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Window::Window(const std::string& title, int width, int height, bool vSync, bool fullScreen)
	: mTitle(title), mWidth(width), mHeight(height), mVSync(vSync)
{
	if (!glfwInit())
	{
		std::cout << "ERROR : GLFW was not initlialized" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(width, height, title.c_str(), fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		std::cout << "ERROR : GLFW window could not be created" << std::endl;
	}

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(mVSync);

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
	// imgui shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


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

bool Window::IsVSync() const
{
	return mVSync;
}

void Window::SetVSync(bool vsync)
{
	mVSync = vsync;
	glfwSwapInterval(mVSync);
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

float Window::GetAspectRatio() const
{
	return ((float)GetWidth() / (float)GetHeight());
}

GLFWwindow* Window::Get() const
{
	return mWindow;
}

bool Window::IsKeyPressed(int keycode)
{
	return glfwGetKey(mWindow, keycode) == GLFW_PRESS;
}

bool Window::IsMousePressed(int keycode)
{
	return glfwGetMouseButton(mWindow, keycode) == GLFW_PRESS;
}

void Window::OnUpdate() const
{
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}