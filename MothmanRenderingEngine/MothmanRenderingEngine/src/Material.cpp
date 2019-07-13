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

void Material::UseMaterial(Shader *shader, const string& specularIntensityUniformName, const string& shininessUniformName)
{
	shader->SetUniform(specularIntensityUniformName, specularIntensity);
	shader->SetUniform(shininessUniformName, shininess);
}

Material::~Material()
{
}