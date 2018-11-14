#include "Material.h"


Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity); //Set provided uniform variable in shader to value set as specularIntensity of the material
	glUniform1f(shininessLocation, shininess); //Set provided uniform variable in shader to value set as shininess of the material
}

Material::~Material()
{
}