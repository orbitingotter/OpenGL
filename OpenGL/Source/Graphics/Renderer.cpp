#include "pch.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>



static void APIENTRY MessageCallback(GLenum source,
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


Renderer::Renderer(Window& window, Camera& camera)
	: mWindowRef(window), mCameraRef(camera)
{
	// Enables debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	// Enables blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enables depth test
	glEnable(GL_DEPTH_TEST);

	// Enables back culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mDefaultShader = std::make_unique<Shader>("Source/Shaders/NormalMap.glsl");
	mDefaultShadowShader = std::make_unique<Shader>("Source/Shaders/ShadowMap.glsl");
	mDefaultCubemapShader = std::make_unique<Shader>("Source/Shaders/Cubemap.glsl");

	mDefaultView = mCameraRef.GetViewMatrix();
	mDefaultProjection = glm::perspective(glm::radians(60.0f), mWindowRef.GetAspectRatio(), 0.1f, 1000.0f);

}


void Renderer::AddSkybox(const std::vector<std::string>& filePaths)
{
	mCubemap = std::make_unique<Cubemap>(filePaths);
}

void Renderer::SetupLights()
{
	// shadows

	// generate shadow map fbo
	glGenFramebuffers(1, &mShadowDescription.shadowMapFBO);

	glGenTextures(1, &mShadowDescription.shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, mShadowDescription.shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mShadowDescription.shadowWidth, mShadowDescription.shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowDescription.shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowDescription.shadowMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Shadow FBO Error\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::Clear(float r, float g, float b, float a) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void Renderer::SetDepthTest(bool enabled)
{
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}


void Renderer::Draw(const VertexArray& vao, int count, const Shader& shader) const
{
	vao.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(Mesh& mesh, Shader& shader)
{
	shader.Bind();
	mesh.BindTextures(shader);
	mesh.Bind();
	glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(Model& model, Shader& shader)
{
	shader.Bind();
	auto& meshes = model.GetMeshes();
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].BindTextures(shader);
		meshes[i].Bind();
		glDrawElements(GL_TRIANGLES, meshes[i].GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::Draw(Cubemap& cubemap, Shader& shader)
{
	cubemap.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, cubemap.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
}


void Renderer::Submit(std::unique_ptr<Model> model, glm::mat4 transform)
{
	mModelList.push_back(std::move(model));
	mTransformList.push_back(transform);
}

void Renderer::DrawSubmitted()
{
	UpdateConfiguration();

	// SHADOW PASS
	if (config.ShadowMapping)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LESS);
		glViewport(0, 0, mShadowDescription.shadowWidth, mShadowDescription.shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, mShadowDescription.shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 smProjection = glm::ortho(-mShadowDescription.shadowOrtho, mShadowDescription.shadowOrtho,
			-mShadowDescription.shadowOrtho, mShadowDescription.shadowOrtho, 0.1f, 100.0f);
		glm::mat4 smLightView = glm::lookAt(50.0f * -glm::normalize(mDirLight.direction), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mDirLight.smLightViewProjection = smProjection * smLightView;

		mDefaultShadowShader->Bind();
		mDefaultShadowShader->SetUniform("uSMProj", mDirLight.smLightViewProjection);

		for (int i = 0; i < mModelList.size(); i++)
		{
			mDefaultShadowShader->SetUniform("uModel", mTransformList[i]);
			Draw(*mModelList[i], *mDefaultShadowShader);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// NORMAL RENDER PASS

	glCullFace(GL_BACK);
	glViewport(0, 0, mWindowRef.GetWidth(), mWindowRef.GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mShadowDescription.shadowMapTexture);

	mDefaultView = mCameraRef.GetViewMatrix();
	mDefaultShader->Bind();
	mDefaultShader->SetUniform("uView", mDefaultView);
	mDefaultShader->SetUniform("uProj", mDefaultProjection);
	mDefaultShader->SetUniform("uCameraPos", mCameraRef.GetPosition());
	mDefaultShader->SetUniform("uDirectionalLight", mDirLight.direction);
	mDefaultShader->SetUniform("uDirectionalColor", mDirLight.color);

	mDefaultShader->SetUniform("uSMProj", mDirLight.smLightViewProjection);
	mDefaultShader->SetUniform("uPcfEnabled", mShadowDescription.pcfEnabled);
	mDefaultShader->SetUniform("uSampleRange", mShadowDescription.sampleRange);
	mDefaultShader->SetUniform("uShadowEnabled", config.ShadowMapping);
	mDefaultShader->SetUniform("uNormalMappingEnabled", config.NomalMapping);

	for (int i = 0; i < mModelList.size(); i++)
	{
		mDefaultShader->SetUniform("uModel", mTransformList[i]);
		Draw(*mModelList[i], *mDefaultShader);
	}

	// CUBEMAP
	mDefaultCubemapShader->Bind();
	mDefaultCubemapShader->SetUniform("uView", glm::mat4(glm::mat3(mDefaultView)));
	mDefaultCubemapShader->SetUniform("uProj", mDefaultProjection);

	glDepthFunc(GL_LEQUAL);
	Draw(*mCubemap, *mDefaultCubemapShader);


}

void Renderer::UpdateConfiguration()
{
	glPolygonMode(GL_FRONT_AND_BACK, config.WireFrame ? GL_LINE : GL_FILL);
}
