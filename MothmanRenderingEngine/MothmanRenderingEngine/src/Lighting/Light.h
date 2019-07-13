#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShadowMap.h"


class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity);

	ShadowMap* GetShadowMap() { return shadowMap; }
	glm::vec3 GetColour() { return colour; }
	GLfloat GetAmbientIntensity() { return ambientIntensity; }
	GLfloat GetDiffuseIntensity() { return diffuseIntensity; }

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj; //View matrix from light persepective
	ShadowMap* shadowMap;
};

