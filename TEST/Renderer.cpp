#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::Clear()
{
	glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.use();
	va.Bind();
	ib.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

