#pragma once

#include <stdio.h>

#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(unsigned int width, unsigned int height);

	virtual void Write();

	virtual void Read(GLenum TextureUnit);

	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();
protected:
	GLuint FBO, shadowMap; //Framebuffer object and shadow map itself
	GLuint shadowWidth, shadowHeight; //Dimensions of shadow map
};