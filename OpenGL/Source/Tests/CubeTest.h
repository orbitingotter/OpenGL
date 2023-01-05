#pragma once

#include "Sandbox.h"

#include "Window.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Renderer.h"

#include <vector>
#include <memory>

class CubeTest : public Sandbox
{
public:
	CubeTest();
	~CubeTest();

	void OnUpdate() override;
	void OnRender() override;
	void OnImGuiRender() override;

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
};