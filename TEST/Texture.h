#pragma once
#include <string>
#include "Renderer.h"
class Texture
{
public:
	Texture();
	~Texture();
	void Add2D(const std::string& path);
	void Active(unsigned int textureUnit);
	void Bind();
	void Unbind();
private:
	unsigned int m_RendererID;
	int width, height, nrChannels;
};
