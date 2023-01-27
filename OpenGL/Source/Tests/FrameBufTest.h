#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"

#include <memory>


class FrameBufTest : public Sandbox
{
public:
	unsigned int fbo;
	unsigned int rbo;
	unsigned int colorTexture;


	FrameBufTest()
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


		glGenFramebuffers(1, &fbo);

		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, window.GetWidth(), window.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.GetWidth(), window.GetHeight());

		// attach textures to framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Incomplete framebuffer\n";
		}

		const float positions[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		const unsigned int indices[] =
		{
			0,1,2,
			0,2,3
		};

		mVAO = std::make_unique<VertexArray>();
		mVBO = std::make_unique<VertexBuffer>(positions, sizeof(positions));
		mIBO = std::make_unique<IndexBuffer>(indices, 6);
		mTextureShader = std::make_unique<Shader>("Source/Shaders/Texture.glsl");
		BufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		mVAO->AddBuffer(*mVBO, *mIBO, layout);

	}

	~FrameBufTest() {}

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


		if (window.IsKeyPressed(GLFW_KEY_ENTER))
		{
			window.Close();
		}

	}

	void OnRender() override
	{
		glEnable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);
		renderer.Draw(*mModel, *mShader);
		renderer.Draw(*mLightModel, *mLightShader);

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexture);

		mTextureShader->Bind();
		mTextureShader->SetUniform("uTexture", 0);
		//mTextureShader->SetUniform("uMVP", mProj * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) *  glm::mat4(1.0f));

		renderer.Draw(*mVAO, mIBO->GetCount(), *mTextureShader);
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

	std::unique_ptr<VertexBuffer> mVBO;
	std::unique_ptr<IndexBuffer> mIBO;
	std::unique_ptr<VertexArray> mVAO;
	std::unique_ptr<Shader> mTextureShader;



	glm::mat4 mModelMat;
	glm::mat4 mView;
	glm::mat4 mProj;

	// light

};