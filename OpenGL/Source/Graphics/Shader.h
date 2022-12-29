#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// manage uniforms
private:
	int GetUniformLocation(const std::string& name);
public:
	void SetUniform(const std::string& name, float v0, float v1, float v2, float v3);

private:
	struct ShaderSource
	{
		std::string vs;
		std::string ps;
	};
	ShaderSource Parse();
	unsigned int Compile(const std::string& shader, unsigned int type);
	void CreateProgram(const std::string& vertexShader, const std::string& pixelShader);

private:
	unsigned int mID;
	std::string mFilePath;
	ShaderSource mSources;
	std::unordered_map<std::string, int> mUniformCache;
};