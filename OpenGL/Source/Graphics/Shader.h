#pragma once

#include <GL/glew.h>
#include <string>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

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
};