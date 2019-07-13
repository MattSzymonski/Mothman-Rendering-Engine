#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat shadowWidth, GLfloat shadowHeight,
	GLfloat near, GLfloat far,
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat con, GLfloat lin, GLfloat exp) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;

	float aspect = (float)shadowWidth / (float)shadowHeight;
	farPlane = far;
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far); //Projection matrix from light source
	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight); //Initialize shadow map

}

std::vector<glm::mat4> PointLight::CalculateLightTransform() //Calculate light transform for each direction
{
	std::vector<glm::mat4> lightTransforms;
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightTransforms;
}

PointLight::~PointLight()
{
}
