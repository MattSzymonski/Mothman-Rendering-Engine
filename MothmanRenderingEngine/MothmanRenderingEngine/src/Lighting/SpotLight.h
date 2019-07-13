#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();

	SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
		GLfloat near, GLfloat far, 
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);

	glm::vec3 GetDirection() { return direction; }
	GLfloat GetEdge() { return edge; }
	GLfloat GetProcEdge() { return procEdge; } 

	~SpotLight();

private:
	glm::vec3 direction;
	GLfloat edge, procEdge;
};

