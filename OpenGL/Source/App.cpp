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
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int main()
{

	Window window("OpenGL App", 1600, 900);
	Renderer renderer;


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


	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
	glm::mat4 projection = glm::ortho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 10.0f);
	projection = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 100.0f);

	view = glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
		1,2,0,	2,1,3,
		4,7,5,	7,4,6,
		0,6,4,	6,0,2,
		5,3,1,  3,5,7,
		2,7,6,	7,2,3,
		1,4,5,	1,0,4
	};

	VertexArray cubeVAO;
	VertexBuffer cubeVBO(cubePos, 6 * 8 * sizeof(float));
	IndexBuffer cubeIBO(cubeIndices, 36);
	BufferLayout cubeLayout;
	cubeLayout.Push<float>(3);
	cubeLayout.Push<float>(3);

	cubeVAO.AddBuffer(cubeVBO, cubeIBO, cubeLayout);


	bool showDemo = true;

	float prev = glfwGetTime();
	int frames = 0;
	float radius = 5.0f;
	float roll = 0.0f, pitch = 0.0f, yaw = 0.0f;

	while (window.IsRunning())
	{
		/* Render here */
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

		float r = sin(glfwGetTime() * 0.2f);
		float g = sin(glfwGetTime() * 0.1f);
		float b = sin(glfwGetTime() * 0.5f);

		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
		view = glm::lookAt(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		shader.SetUniform("uColor", r, g, b, 1.0f);
		shader.SetUniform("uMVP", projection * view * model);

		//renderer.Draw(vao, ibo.GetCount(), shader);

		renderer.Draw(cubeVAO, cubeIBO.GetCount(), shader);


		if (glfwGetKey(window.Get(), GLFW_KEY_ENTER))
		{
			window.Close();
		}

		float now = glfwGetTime();
		if (now - prev > 1.0f)
		{
			std::string title = " FPS : " + std::to_string(frames) + " Delta : " + std::to_string(1.0f / frames * 1000).substr(0, 4) + "ms";
			window.SetTitle(title);
			prev = now;
			frames = 0;
		}
		frames++;


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (showDemo)
			ImGui::ShowDemoWindow(&showDemo);

		static bool testWindow = true;
		if (testWindow)
		{
			ImGui::Begin("View Matrix", &testWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::SliderFloat("Radius", &radius, -20.0f, 20.0f);
			ImGui::Text("FPS : %.1f FPS | Delta : %.2f ms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

			if (ImGui::Button("VSync"))
			{
				if (window.IsVSync())
					window.SetVSync(false);
				else
					window.SetVSync(true);
			}

			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -radius));


			ImGui::End();
		}

		// render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.OnUpdate();
	}


	return 0;
}