#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer();

	void Clear(float r, float g, float b, float a) const;
public:
	void Draw(const VertexArray& vao, int count, const Shader& shader) const;
};