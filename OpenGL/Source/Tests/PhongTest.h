#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




class PhongTest : public Sandbox
{
public:
	PhongTest()
	{/*
		mVertex =
		{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		for (int i = 0; i < 36; i++)
		{
			mIndices.push_back(i);
		}*/


		//______________

		Assimp::Importer imp;
		const aiScene* model = imp.ReadFile("Resources/Models/standford-bunny.stl", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		const auto pMesh = model->mMeshes[0];
		const float scale = 0.02f;

		mIndices.reserve(pMesh->mNumVertices * 6);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			mVertex.emplace_back(scale * pMesh->mVertices[i].x);
			mVertex.emplace_back(scale * pMesh->mVertices[i].y);
			mVertex.emplace_back(scale * pMesh->mVertices[i].z);
			mVertex.emplace_back(pMesh->mNormals[i].x);
			mVertex.emplace_back(pMesh->mNormals[i].y);
			mVertex.emplace_back(pMesh->mNormals[i].z);
		}
		mIndices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& pFace = pMesh->mFaces[i];
			mIndices.emplace_back(pFace.mIndices[0]);
			mIndices.emplace_back(pFace.mIndices[1]);
			mIndices.emplace_back(pFace.mIndices[2]);

		}
		std::cout << mVertex.size() << " " << mIndices.size() << std::endl;
		//______________


		mVAO = std::make_unique<VertexArray>();
		mVBO = std::make_unique<VertexBuffer>(mVertex.data(), mVertex.size() * sizeof(float));
		mIBO = std::make_unique<IndexBuffer>(mIndices.data(), mIndices.size());
		BufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		mVAO->AddBuffer(*mVBO, *mIBO, layout);
		mShader = std::make_unique<Shader>("Source/Shaders/Phong.glsl");

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

	~PhongTest() {}

	void OnUpdate() override
	{
		//mModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(1, 1, 1));
		mLightModel = glm::translate(glm::mat4(1.0f), mLightPos);
		mLightModel = glm::scale(mLightModel, glm::vec3(0.2f));
		mView = camera.GetViewMatrix();

		mShader->Bind();
		mShader->SetUniform("uModel", mModel);
		mShader->SetUniform("uView", mView);
		mShader->SetUniform("uProj", mProj);
		mShader->SetUniform("uLightPos", mLightPos);
		mShader->SetUniform("uCameraPos", camera.GetPosition());

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