#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Graphics/Cubemap.h"

#include <memory>


class CurrentTest : public Sandbox
{
public:
	CurrentTest()
	{
		float start = glfwGetTime();
		mModel = std::make_unique<Model>("Resources/Models/sponza/sponza.obj");
		std::cout << "Model took " << glfwGetTime() - start << "s to load\n";
		mModelMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));

		mVertexCount = mModel->GetVertexCount();
		mIndexCount = mModel->GetIndexCount();
		mTextureCount = mModel->GetTextureCount();
		mMeshCount = mModel->GetNumMeshes();

		std::vector<std::string> facePaths =
		{
			"Resources/Skybox/Sky/right.png",
			"Resources/Skybox/Sky/left.png",
			"Resources/Skybox/Sky/top.png",
			"Resources/Skybox/Sky/bottom.png",
			"Resources/Skybox/Sky/front.png",
			"Resources/Skybox/Sky/back.png"
		};

		renderer.AddSkybox(facePaths);

		renderer.AddLight(glm::vec3(-0.3f, -1.0f, -0.1f));
		renderer.SetupLights();

		renderer.Submit(std::move(mModel), mModelMat);


	}

	~CurrentTest() {}

	void OnUpdate() override
	{

		if (window.IsKeyPressed(GLFW_KEY_ENTER))
		{
			window.Close();
		}

	}

	void OnRender() override
	{
		renderer.Clear(0.1f, 0.1f, 0.1f, 0.0f);
		renderer.DrawSubmitted();
	}

	void OnImGuiRender() override
	{
		static bool testWindow = true;
		if (testWindow)
		{
			ImGui::Begin("Info", &testWindow);

			if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent(16.0f);

				ImGui::Text("FPS : %.1f FPS | Delta : %.2f ms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
				static bool vsync = window.IsVSync();
				ImGui::Checkbox("VSync", &vsync);
				window.SetVSync(vsync);

				// FPS Plotter
				{
					const int max = 150;
					static float fpsSamples[max];
					static float scaleFactor = 0.0f;

					for (int i = 0; i < max - 1; i++)
					{
						fpsSamples[i] = fpsSamples[i + 1];
					}

					fpsSamples[max - 1] = ImGui::GetIO().Framerate;
					if (scaleFactor < ImGui::GetIO().Framerate)
						scaleFactor = ImGui::GetIO().Framerate;

					ImGui::PlotLines("Lines", fpsSamples, 100, 0, 0, 0.0f, scaleFactor * 1.1f, ImVec2(500.0f, 100.0f));
				}

				ImGui::Text("Vertex Count : %d", mVertexCount);
				ImGui::Text("Index Count : %d", mIndexCount);
				ImGui::Text("Texture Count : %d", mTextureCount);
				ImGui::Text("Mesh Count : %d", mMeshCount);

				ImGui::Unindent();
			}

			ImGui::Separator();


			if (ImGui::CollapsingHeader("Render Configuration", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent(16.0f);

				ImGui::Checkbox("Wireframe", &renderer.config.WireFrame);
				ImGui::Checkbox("Shadow Mapping", &renderer.config.ShadowMapping);
				ImGui::Checkbox("Normal Mapping", &renderer.config.NomalMapping);
				ImGui::Checkbox("Parallax Mapping", &renderer.config.ParallaxMapping);

				ImGui::Unindent();
			}

			ImGui::Separator();

			if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent(16.0f);

				Renderer::ShadowMapDesc& desc = renderer.GetShadowDescription();
				ImGui::DragFloat3("Directional Light", &renderer.GetLight().direction.x, 0.05f);
				ImGui::ColorEdit3("Directional Color", &renderer.GetLight().color.x);
				ImGui::DragFloat("Directional Intensity", &renderer.GetLight().intensity, 0.1f, 0.0f, 200.0f);

				ImGui::DragFloat("Ortho", &desc.shadowOrtho, 1.0f, 5.0f, 500.0f);
				ImGui::Checkbox("PCF enabled", &desc.pcfEnabled);
				ImGui::DragInt("Sample Range", &desc.sampleRange, 2.0f, 1, 15);
				ImGui::Text("Num Samples : %d", desc.sampleRange * desc.sampleRange);
				ImGui::Image((ImTextureID)desc.shadowMapTexture, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));

				ImGui::Unindent();
			}

			ImGui::Separator();

			if (ImGui::CollapsingHeader("Post Processing", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Indent(16.0f);

				ImGui::Checkbox("Gamma Correction", &renderer.config.PostProcessor.GammaCorrection);

				ImGui::Combo("Tonemapping", reinterpret_cast<int*>(&renderer.config.PostProcessor.ToneMappingAlg),
					"Reinhard\0Simple Exposure\0");

				if(renderer.config.PostProcessor.ToneMappingAlg == Renderer::ToneMappingAlg::Exposure)
					ImGui::DragFloat("Exposure", &renderer.config.PostProcessor.Exposure, 0.005f, 0.0f, 10.0f);

				ImGui::Unindent();
			}

			ImGui::End();
		}
	}

private:

	std::unique_ptr<Model> mModel;

	glm::mat4 mModelMat;

	int mVertexCount, mIndexCount, mTextureCount, mMeshCount;

};