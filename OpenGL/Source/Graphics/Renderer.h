#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Framebuffer.h"
#include "Cubemap.h"

#include "DirectionalLight.h"

#include "Camera.h"
#include "Window.h"

class Renderer
{
public:
	Renderer(Window& window, Camera& camera);


	void Clear(float r, float g, float b, float a) const;
	void SetDepthTest(bool enabled);

	void AddLight(DirectionalLight light) { mDirLight = light; }
	void SetupLights();
	void AddSkybox(const std::vector<std::string>& filePaths);

	void Draw(const VertexArray& vao, int count, const Shader& shader) const;
	void Draw(Mesh& mesh, Shader& shader);
	void Draw(Model& model, Shader& shader);
	void Draw(Cubemap& cubemap, Shader& shader);

	void Submit(std::unique_ptr<Model> model, glm::mat4 modelMat);
	void DrawSubmitted();

	void UpdateConfiguration();

public:
	struct ShadowMapDesc
	{
		unsigned int shadowMapFBO = -1;
		unsigned int shadowMapTexture = -1;
		const int shadowWidth = 4096, shadowHeight = 4096;
		float shadowOrtho = 20.0f;

		bool pcfEnabled = true;
		int sampleRange = 5;
	};

	struct Configuration
	{
		bool WireFrame = false;

		bool ShadowMapping = true;
		bool NomalMapping = true;
		bool ParallaxMapping = false;

		bool GammaCorrection = true;
		float exposure = 1.0f;
	};


	ShadowMapDesc& GetShadowDescription() { return mShadowDescription; }
	DirectionalLight& GetLight() { return mDirLight; }

public:

	Configuration config;
private:


	Window& mWindowRef;
	Camera& mCameraRef;

	std::vector<std::unique_ptr<Model>> mModelList;
	std::vector<glm::mat4> mTransformList;

	std::unique_ptr<Shader> mDefaultShader;
	std::unique_ptr<Shader> mDefaultShadowShader;
	std::unique_ptr<Shader> mDefaultCubemapShader;

	std::unique_ptr<Cubemap> mCubemap;

	glm::mat4 mDefaultView;
	glm::mat4 mDefaultProjection;


	// shadows
	DirectionalLight mDirLight;
	ShadowMapDesc mShadowDescription;

	// post process
	std::unique_ptr<Model> mPostProcessQuad;
	std::unique_ptr<Framebuffer> mFrameBuf;
	std::unique_ptr<Shader> mDefaultPostProcessShader;
};