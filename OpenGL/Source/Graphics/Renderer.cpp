#include "Renderer.h"

void Renderer::Clear(float r, float g, float b, float a) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void Renderer::Draw(const VertexArray& vao, int count, const Shader& shader) const
{
	vao.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
