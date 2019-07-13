#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	std::vector<glm::mat4> CalculateLightTransform();
	glm::vec3 GetPosition() { return position; }
	void SetPosition(glm::vec3 newPosition) { position = newPosition; }

	GLfloat GetFarPlane() { return farPlane; }
	GLfloat GetConstant() { return constant; }
	GLfloat GetLinear() { return linear; }
	GLfloat GetExponent() { return exponent; }
	
	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
	GLfloat farPlane;
};

