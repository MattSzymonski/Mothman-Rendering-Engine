#include "Light.h"


Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); //Light do not color the object, it just show color parts of the object, pure white light color - all colors in 100% intensity will be shown
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;

	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

Light::~Light()
{
}
