#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const std::string& title, int width, int height);
	~Window();

	bool IsRunning() const;
	void Close() const;


	// getters and setters
	void SetVSync(bool vsync) const;

	const std::string& GetTitle() const;
	void SetTitle(const std::string& title);

	void SetSize(int x, int y);
	int GetWidth() const;
	int GetHeight() const;

	GLFWwindow* Get() const;

	void OnUpdate() const;
private:
	std::string mTitle;
	int mWidth, mHeight;
	GLFWwindow* mWindow;
};