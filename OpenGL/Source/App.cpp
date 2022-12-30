#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void APIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << std::endl;

	// OpenGL Source
	std::cout << "Source: ";
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
	}
	std::cout << std::endl;

	// Debug Message
	std::cout << "Message: " << message << std::endl;

	// Error Type
	std::cout << "Type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
	}

	// Error ID
	std::cout << std::endl;
	std::cout << "Id: " << id << std::endl;

	// Error Severity
	std::cout << "Severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "NOTIFICATION";
		break;
	}
	std::cout << std::endl;

	// Backtrack error location
#ifdef _DEBUG
	__debugbreak();
#endif
}

int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 720, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	fprintf(stdout, "GLEW : Using GLEW %s\n", glewGetString(GLEW_VERSION));
	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;


	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);


	float positions[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f,  0.0f, 1.0f,
		0.5f, 0.5f,   1.0f, 1.0f,
		0.5f , -0.5f,  1.0f, 0.0f
	};

	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	VertexArray vao;
	VertexBuffer vbo(positions, 4 * 4 * sizeof(float));
	IndexBuffer ibo(indices, 6);
	BufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	vao.AddBuffer(vbo, ibo, layout);

	Shader shader("Source/Shaders/Basic.glsl");

	Texture texture("Resources/ghibli.png");
	texture.Bind();

	shader.Bind();
	shader.SetUniform("uTexture", 0);

	Renderer renderer;

	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 projection = glm::ortho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.33f, 0.1f, 100.0f);

	shader.SetUniform("uMVP", projection * view * model);



	float cubePos[] =
	{
		-0.5f, -0.5f, -0.5f,	0.4f, 0.8f, 0.1f,
		-0.5f, -0.5f, +0.5f,	0.8f, 0.6f, 0.1f,
		-0.5f, +0.5f, -0.5f,	0.2f, 0.6f, 0.8f,
		-0.5f, +0.5f, +0.5f,	0.5f, 0.6f, 0.2f,
		+0.5f, -0.5f, -0.5f,	0.2f, 0.8f, 0.1f,
		+0.5f, -0.5f, +0.5f,	0.8f, 0.6f, 0.1f,
		+0.5f, +0.5f, -0.5f,	0.2f, 0.6f, 0.5f,
		+0.5f, +0.5f, +0.5f,	0.9f, 0.1f, 0.7f,
	};

	unsigned int cubeIndices[] =
	{
		0,1,2,	1,2,3,
		4,5,6,	5,6,7,
		0,6,2,	0,4,6,
		1,7,3,  1,5,7,
		2,6,7,	2,3,7,
		0,4,5,	0,1,5
	};

	VertexArray cubeVAO;
	VertexBuffer cubeVBO(cubePos, 6 * 8 * sizeof(float));
	IndexBuffer cubeIBO(cubeIndices, 36);
	BufferLayout cubeLayout;
	cubeLayout.Push<float>(3);
	cubeLayout.Push<float>(3);

	cubeVAO.AddBuffer(cubeVBO, cubeIBO, cubeLayout);




	/* Loop until the user closes the window */
	float prev = glfwGetTime();
	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

		float r = sin(glfwGetTime() * 0.2f);
		float g = sin(glfwGetTime() * 0.1f);
		float b = sin(glfwGetTime() * 0.5f);

		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
		shader.SetUniform("uColor", r, g, b, 1.0f);
		shader.SetUniform("uMVP", projection * view * model);

		renderer.Draw(cubeVAO, cubeIBO.GetCount(), shader);
		//renderer.Draw(vao, ibo.GetCount(), shader);


		if (glfwGetKey(window, GLFW_KEY_ENTER))
		{
			glfwSetWindowShouldClose(window, true);
		}

		float now = glfwGetTime();
		if (now - prev > 1.0f)
		{
			std::string title = " FPS : " + std::to_string(frames) + " Delta : " + std::to_string(1.0f / frames * 1000).substr(0, 4) + "ms";
			glfwSetWindowTitle(window, title.c_str());
			prev = now;
			frames = 0;
		}
		frames++;


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}


	glfwTerminate();

	return 0;
}