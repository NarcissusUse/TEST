#pragma once

#include <glad\glad.h>

#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "shader.h"
#include "IndexBuffer.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
class Renderer
{
public:
	Renderer();
	void Clear();
	void Draw(const VertexArray& va,const IndexBuffer& ib,const Shader& shader);

private:

};
