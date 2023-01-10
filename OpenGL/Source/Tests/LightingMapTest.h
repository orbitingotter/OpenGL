#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture2D.h"


class LightingMapTest : public Sandbox
{
public:
	LightingMapTest()
	{
		mVertex =
		{
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		for (int i = 0; i < 36; i++)
		{
			mIndices.push_back(i);
		}


		mVAO = std::make_unique<VertexArray>();
		mVBO = std::make_unique<VertexBuffer>(mVertex.data(), mVertex.size() * sizeof(float));
		mIBO = std::make_unique<IndexBuffer>(mIndices.data(), mIndices.size());
		BufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		layout.Push<float>(2);
		mVAO->AddBuffer(*mVBO, *mIBO, layout);
		mShader = std::make_unique<Shader>("Source/Shaders/LightingMap.glsl");
		mDiffuseTexture = std::make_unique<Texture2D>("Resources/container-diffuse.png");
		mSpecularTexture = std::make_unique<Texture2D>("Resources/container-specular.png");
		mEmissiveTexture = std::make_unique<Texture2D>("Resources/container-emissive2.png");

		mModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		mView = camera.GetViewMatrix();
		mProj = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 0.1f, 100.0f);


		// light cube
		mLightPos = { 1.0f, 1.0f, 0.0f };
		mLightModel = glm::mat4(1.0f);
		float lightPos[] =
		{
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, +0.5f,
			-0.5f, +0.5f, -0.5f,
			-0.5f, +0.5f, +0.5f,
			+0.5f, -0.5f, -0.5f,
			+0.5f, -0.5f, +0.5f,
			+0.5f, +0.5f, -0.5f,
			+0.5f, +0.5f, +0.5f
		};

		unsigned int lightIndices[] =
		{
			1,2,0,	2,1,3,
			4,7,5,	7,4,6,
			0,6,4,	6,0,2,
			5,3,1,  3,5,7,
			2,7,6,	7,2,3,
			1,4,5,	1,0,4
		};

		lightVAO = std::make_unique<VertexArray>();
		lightVBO = std::make_unique<VertexBuffer>(lightPos, 3 * 8 * sizeof(float));
		lightIBO = std::make_unique<IndexBuffer>(lightIndices, 6 * 6);
		BufferLayout lightLayout;
		lightLayout.Push<float>(3);
		lightVAO->AddBuffer(*lightVBO, *lightIBO, lightLayout);
		lightShader = std::make_unique<Shader>("Source/Shaders/SolidBasic.glsl");
	}

	~LightingMapTest() {}

	void OnUpdate() override
	{
		mModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
		mLightModel = glm::translate(glm::mat4(1.0f), mLightPos);
		mLightModel = glm::scale(mLightModel, glm::vec3(0.2f));
		mView = camera.GetViewMatrix();

		mShader->Bind();
		mShader->SetUniform("uModel", mModel);
		mShader->SetUniform("uView", mView);
		mShader->SetUniform("uProj", mProj);
		mShader->SetUniform("uLightPos", mLightPos);
		mShader->SetUniform("uCameraPos", camera.GetPosition());
		mDiffuseTexture->Bind(0);
		mShader->SetUniform("uDiffuseTex", 0);
		mSpecularTexture->Bind(1);
		mShader->SetUniform("uSpecularTex", 1);
		//mEmissiveTexture->Bind(2);
		//mShader->SetUniform("uEmissiveTex", 2);

		lightShader->Bind();
		lightShader->SetUniform("uModel", mLightModel);
		lightShader->SetUniform("uView", mView);
		lightShader->SetUniform("uProj", mProj);

		if (window.IsKeyPressed(GLFW_KEY_ENTER))
		{
			window.Close();
		}

	}

	void OnRender() override
	{
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);

		renderer.Draw(*mVAO, mIBO->GetCount(), *mShader);

		renderer.Draw(*lightVAO, lightIBO->GetCount(), *lightShader);
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

			ImGui::SliderFloat("Light X", &mLightPos.x, -20.0f, 20.0f);
			ImGui::SliderFloat("Light Y", &mLightPos.y, -20.0f, 20.0f);
			ImGui::SliderFloat("Light Z", &mLightPos.z, -20.0f, 20.0f);


			ImGui::End();
		}
	}

private:
	std::unique_ptr<VertexArray> mVAO;
	std::unique_ptr<VertexBuffer> mVBO;
	std::unique_ptr<IndexBuffer> mIBO;
	std::unique_ptr<Shader> mShader;
	std::unique_ptr<Texture2D> mDiffuseTexture;
	std::unique_ptr<Texture2D> mSpecularTexture;
	std::unique_ptr<Texture2D> mEmissiveTexture;

	std::vector<float> mVertex;
	std::vector<unsigned int> mIndices;

	glm::mat4 mModel;
	glm::mat4 mView;
	glm::mat4 mProj;

	// light
	glm::vec3 mLightPos;
	glm::mat4 mLightModel;
	std::unique_ptr<VertexArray> lightVAO;
	std::unique_ptr<VertexBuffer> lightVBO;
	std::unique_ptr<IndexBuffer> lightIBO;
	std::unique_ptr<Shader> lightShader;

};