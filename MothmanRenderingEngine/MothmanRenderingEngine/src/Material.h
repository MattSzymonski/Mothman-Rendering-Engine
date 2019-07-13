#pragma once

#include <GL\glew.h>
#include "Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(Shader *shader, const string& specularIntensityUniformName, const string& shininessUniformName);

	~Material();

private:
	GLfloat specularIntensity; //How bright the light reflected is
	GLfloat shininess; //How much the reflection is concentrated or diffused
};
