#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Cubemap.h"

#include <memory>


class CubemapTest : public Sandbox
{
public:
	CubemapTest()
	{
		mShader = std::make_unique<Shader>("Source/Shaders/Model.glsl");
		float start = glfwGetTime();
		mModel = std::make_unique<Model>("Resources/Models/sponza/sponza.obj");
		std::cout << "Model took " << glfwGetTime() - start << "s to load\n";

		mLightShader = std::make_unique<Shader>("Source/Shaders/SolidBasic.glsl");
		mLightModel = std::make_unique<Model>("Resources/Models/cube.obj");
		mLightPos = glm::vec3(0.0f, 0.0f, -2.0f);

		mModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
		mView = camera.GetViewMatrix();
		mProj = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 1000.0f);

		std::vector<std::string> facePaths =
		{
			"Resources/Skybox/Sky/right.png",
			"Resources/Skybox/Sky/left.png",
			"Resources/Skybox/Sky/top.png",
			"Resources/Skybox/Sky/bottom.png",
			"Resources/Skybox/Sky/front.png",
			"Resources/Skybox/Sky/back.png"
		};

		mCubemap = std::make_unique<Cubemap>(facePaths);
		mCubemapShader = std::make_unique<Shader>("Source/Shaders/Cubemap.glsl");

	}

	~CubemapTest() {}

	void OnUpdate() override
	{
		mView = camera.GetViewMatrix();

		mShader->Bind();
		mShader->SetUniform("uModel", mModelMat);
		mShader->SetUniform("uView", mView);
		mShader->SetUniform("uProj", mProj);
		mShader->SetUniform("uLightPos", mLightPos);
		mShader->SetUniform("uCameraPos", camera.GetPosition());

		const float radius = 5.0f;
		mLightPos.x = radius * sin(glfwGetTime());
		mLightPos.z = radius * cos(glfwGetTime());
		mLightPos.y = 10.0f;

		mLightShader->Bind();
		mLightShader->SetUniform("uModel", glm::scale(glm::translate(glm::mat4(1.0f), mLightPos), glm::vec3(0.2f)));
		mLightShader->SetUniform("uView", mView);
		mLightShader->SetUniform("uProj", mProj);

		mCubemapShader->Bind();
		mCubemapShader->SetUniform("uView", glm::mat4(glm::mat3(mView)));
		mCubemapShader->SetUniform("uProj", mProj);



		if (window.IsKeyPressed(GLFW_KEY_ENTER))
		{
			window.Close();
		}

	}

	void OnRender() override
	{
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

		//glDepthFunc(GL_LESS);
		renderer.Draw(*mModel, *mShader);
		renderer.Draw(*mLightModel, *mLightShader);

		glDepthFunc(GL_LEQUAL);
		renderer.Draw(*mCubemap, *mCubemapShader);
	}

	void OnImGuiRender() override
	{
		static bool testWindow = true;
		if (testWindow)
		{
			ImGui::Begin("Info", &testWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("FPS : %.1f FPS | Delta : %.2f ms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);

			if (ImGui::Button("VSync"))
			{
				if (window.IsVSync())
					window.SetVSync(false);
				else
					window.SetVSync(true);
			}

			if (ImGui::Button("Wireframe ON"))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}

			if (ImGui::Button("Wireframe OFF"))
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			ImGui::Text("Vertex Count : %d", mModel->GetVertexCount());
			ImGui::Text("Index Count : %d", mModel->GetIndexCount());
			ImGui::Text("Texture Count : %d", mModel->GetTextureCount());
			ImGui::Text("Mesh Count : %d", mModel->GetNumMeshes());

			ImGui::End();
		}
	}

private:

	std::unique_ptr<Model> mModel;
	std::unique_ptr<Shader> mShader;

	std::unique_ptr<Model> mLightModel;
	std::unique_ptr<Shader> mLightShader;
	glm::vec3 mLightPos;

	std::unique_ptr<Cubemap> mCubemap;
	std::unique_ptr<Shader> mCubemapShader;


	glm::mat4 mModelMat;
	glm::mat4 mView;
	glm::mat4 mProj;

	// light

};