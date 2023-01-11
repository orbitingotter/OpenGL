#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"

#include <memory>


class ModelTest : public Sandbox
{
public:
	ModelTest()
	{
		mShader = std::make_unique<Shader>("Source/Shaders/Model.glsl");
		mModel = std::make_unique<Model>("Resources/backpack/backpack.obj");

		mModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		mView = camera.GetViewMatrix();
		mProj = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 100.0f);

	}

	~ModelTest() {}

	void OnUpdate() override
	{
		mView = camera.GetViewMatrix();

		mShader->Bind();
		mShader->SetUniform("uModel", mModelMat);
		mShader->SetUniform("uView", mView);
		mShader->SetUniform("uProj", mProj);


		if (window.IsKeyPressed(GLFW_KEY_ENTER))
		{
			window.Close();
		}

	}

	void OnRender() override
	{
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

		// Draw the box
		renderer.Draw(*mModel, *mShader);


	}

	void OnImGuiRender() override
	{
		static bool testWindow = true;
		if (testWindow)
		{
			ImGui::Begin("Light", &testWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
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
			ImGui::Text("Mesh Count : %d", mModel->GetNumMeshes());

			ImGui::End();
		}
	}

private:

	std::unique_ptr<Model> mModel;
	std::unique_ptr<Shader> mShader;

	glm::mat4 mModelMat;
	glm::mat4 mView;
	glm::mat4 mProj;

	// light

};