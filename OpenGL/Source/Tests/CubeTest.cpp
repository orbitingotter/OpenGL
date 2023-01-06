#include "CubeTest.h"

#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

CubeTest::CubeTest()
{
	mVertex =
	{
		-0.5f, -0.5f, -0.5f,	0.4f, 0.8f, 0.1f,
		-0.5f, -0.5f, +0.5f,	0.8f, 0.6f, 0.1f,
		-0.5f, +0.5f, -0.5f,	0.2f, 0.6f, 0.8f,
		-0.5f, +0.5f, +0.5f,	0.5f, 0.6f, 0.2f,
		+0.5f, -0.5f, -0.5f,	0.2f, 0.8f, 0.1f,
		+0.5f, -0.5f, +0.5f,	0.8f, 0.6f, 0.1f,
		+0.5f, +0.5f, -0.5f,	0.2f, 0.6f, 0.5f,
		+0.5f, +0.5f, +0.5f,	0.9f, 0.1f, 0.7f
	};

	mIndices =
	{
		1,2,0,	2,1,3,
		4,7,5,	7,4,6,
		0,6,4,	6,0,2,
		5,3,1,  3,5,7,
		2,7,6,	7,2,3,
		1,4,5,	1,0,4
	};

	mVAO = std::make_unique<VertexArray>();
	mVBO = std::make_unique<VertexBuffer>(mVertex.data(), 6 * 8 * sizeof(float));
	mIBO = std::make_unique<IndexBuffer>(mIndices.data(), mIndices.size());
	BufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	mVAO->AddBuffer(*mVBO, *mIBO, layout);

	mShader = std::make_unique<Shader>("Source/Shaders/Basic.glsl");

	mModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	mView = glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mProj = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 100.0f);


}

CubeTest::~CubeTest()
{
}

static float radius = 5.0f;

void CubeTest::OnUpdate()
{

	float r = (float)sin(glfwGetTime() * 0.2f);
	float g = (float)sin(glfwGetTime() * 0.1f);
	float b = (float)sin(glfwGetTime() * 0.5f);

	mModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
	mView = glm::lookAt(glm::vec3(sin(glfwGetTime()) * radius, 0.0f, cos(glfwGetTime()) * radius), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	mShader->SetUniform("uColor", r, g, b, 1.0f);
	mShader->SetUniform("uMVP", mProj * mView * mModel);

	if (window.IsKeyPressed(GLFW_KEY_ENTER))
	{
		window.Close();
	}

}

void CubeTest::OnRender()
{
	renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

	renderer.Draw(*mVAO, mIBO->GetCount(), *mShader);
}

void CubeTest::OnImGuiRender()
{
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

		mView = glm::translate(mView, glm::vec3(0.0f, 0.0f, -radius));


		ImGui::End();
	}
}
