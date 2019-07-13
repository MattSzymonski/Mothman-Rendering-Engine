#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {}

bool OmniShadowMap::Init(unsigned int width, unsigned int height)
{
	shadowWidth = width; shadowHeight = height;

	glGenFramebuffers(1, &FBO); //Generate framebuffer object name (generated name will be stored in FBO)

	glGenTextures(1, &shadowMap); //Generate texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap); //Bind cube map texture

	for (size_t i = 0; i < 6; ++i) //Depth texture initialization, 6 times because cube map has 6 sides
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	//Texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO); //Bind FBO(generated FBO name) to the framebuffer [There is only one framebuffer! we just change where it is writing data to!]
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0); //Connect the framebuffer to the texture (not 2d) so if framebuffer got updated result will be stored in a texture

	glDrawBuffer(GL_NONE); //Draw scene (only depth)
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER); //Get framebuffer status
	if (Status != GL_FRAMEBUFFER_COMPLETE) //Check if framebuffer completed drawing
	{
		printf("Framebuffer error: %i\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

OmniShadowMap::~OmniShadowMap()
{
}
