#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
}

bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO); //Generate framebuffer object name (generated name will be stored in FBO)

	glGenTextures(1, &shadowMap); //Generate texture
	glBindTexture(GL_TEXTURE_2D, shadowMap); //Bind texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); //Depth texture initialization 

	//Texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST (gives more pixelated look)
	//Set regions outside texture to plain white color (so no shadows there) 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); //Bind FBO(generated FBO name) to the framebuffer [There is only one framebuffer! we just change where it is writing data to!]
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0); //Connect the framebuffer to the texture so if framebuffer got updated result will be stored in a texture

	glDrawBuffer(GL_NONE); //Draw scene (only depth)
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER); //Get framebuffer status
	if (Status != GL_FRAMEBUFFER_COMPLETE) //Check if framebuffer completed drawing
	{
		printf("Framebuffer error: %s\n", Status);
		return false;
	}

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); //Bind FBO(generated FBO name) to the framebuffer [There is only one framebuffer! we just change where it is writing data to!]
}


void ShadowMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit); //Specifies which texture unit to make active
	glBindTexture(GL_TEXTURE_2D, shadowMap); //Bind shadow map texture to active texture unit
}

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}
