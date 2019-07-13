#pragma once

#include <stdio.h>
#include <iostream>
#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(unsigned int width, unsigned int height);

	GLuint GetFBO() { return FBO; }
	GLuint GetShadowMapTexture() { return shadowMap; }
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();
protected:
	GLuint shadowMap; //Shadow map texture
	GLuint FBO; //Framebuffer object 
	GLuint shadowWidth, shadowHeight; //Dimensions of shadow map
};