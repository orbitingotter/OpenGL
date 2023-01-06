#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(const std::string& title, int width, int height, bool vSync = true, bool fullScreen = false);
	~Window();

	bool IsRunning() const;
	void Close() const;

	void OnUpdate() const;

	// getters and setters
	bool IsVSync() const;
	void SetVSync(bool vsync);

	const std::string& GetTitle() const;
	void SetTitle(const std::string& title);

	void SetSize(int x, int y);
	int GetWidth() const;
	int GetHeight() const;
	float GetAspectRatio() const;

	GLFWwindow* Get() const;

	// input polling
	bool IsKeyPressed(int keycode);
	bool IsMousePressed(int keycode);

private:
	std::string mTitle;
	int mWidth, mHeight;
	GLFWwindow* mWindow;
	bool mVSync;
};