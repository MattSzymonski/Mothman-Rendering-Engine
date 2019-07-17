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
	glm::vec3 GetColor() { return color; }
	void SetColor(glm::vec3 value) { color = value; }
	GLfloat GetAmbientIntensity() { return ambientIntensity; }
	void SetAmbientIntensity(GLfloat value) { ambientIntensity = value; }
	GLfloat GetDiffuseIntensity() { return diffuseIntensity; }
	void SetDiffuseIntensity(GLfloat value) { diffuseIntensity = value; }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj; //View matrix from light persepective
	ShadowMap* shadowMap;
};

