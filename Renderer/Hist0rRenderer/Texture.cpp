#include "Texture.h"


Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

bool Texture::LoadTexture()
{
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0); //Loading texture (One char is one byte so char array is array of bytes)
	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); //Generate texture and apply ID to it
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Setting parameters of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Wrapping x-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Wrapping y-axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Zoomed in
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Zoomed out


	if (bitDepth == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData); //Load data to binded texture RGB
	}
	else 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //Load data to binded texture RGB+A
	}

	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0); //Unbind texture

	stbi_image_free(texData); //Clear data because it was loaded to the texture and we dont need it anymore

	return true;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0); //(GL_TEXTURE0 - Texture Unit. min 16 on modern GPU) Setting Texture Unit
	glBindTexture(GL_TEXTURE_2D, textureID); //Binding texture with given ID to Texture Unit in line above
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}


Texture::~Texture()
{
	ClearTexture();
}
