#pragma once

#include <GL\glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);



	bool LoadTexture(); //Load texture without alpha channel
	bool LoadTextureA(); //Load texture with alpha channel

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

