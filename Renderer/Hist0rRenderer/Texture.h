#pragma once

#include <GL\glew.h>
#include "CommonValues.h"

enum TexType { None, Diffuse, Normal, Heightmap };

class Texture
{
public:

	Texture();
	Texture(const char* fileLoc, TexType texType);


	bool LoadTexture(); //Load texture without alpha channel

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	

	GLuint textureID;
	int width, height, bitDepth;
	TexType texType;
	const char* fileLocation;
};

