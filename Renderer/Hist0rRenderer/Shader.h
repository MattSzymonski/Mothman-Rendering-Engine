#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"

#include "Terrain\TerrainNode.h"
#include "Terrain\TerrainConfig.h"

class TerrainNode;
class TerrainConfig;

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* tessellationControlLocation, const char* tessellationEvaluationLocation, const char* geometryLocation, const char* fragmentLocation); //For Terrain

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColourLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetCameraPositionLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();

	void SetDirectionalLight(DirectionalLight * dLight);
	void SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);


	//Shadow mapping
	void SetTextureDiffuse(GLuint textureUnit); 
	void SetTextureNormal(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);

	void SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices);

	void UseShader();
	void ClearShader();

	~Shader();


	void UpdateTerrainUniforms(TerrainNode *node);

private:
	int pointLightCount;
	int spotLightCount;

	//Pointers to uniform variables in shaders
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformCameraPosition,
		uniformSpecularIntensity, uniformShininess,
		uniformTextureDiffuse, uniformTextureNormal,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	//Terrain
	GLuint terrain_uniformCameraPosition, terrain_uniformScaleY, terrain_uniformLod, terrain_uniformIndex, terrain_uniformLocalMatrix, terrain_uniformWorldMatrix, terrain_uniformGap, terrain_uniformLocation;
	GLuint terrain_uniformMorphArea[8];
	GLuint terrain_uniformViewProjection;
	GLuint terrain_tessellationFactor, terrain_tessellationSlope, terrain_tessellationShift;
	GLuint terrain_heightmap;


	GLuint uniformLightMatrices[6];


	//Directional light structure
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;


	
	//Point light structure
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];
	
	GLuint uniformPointLightCount;



	//Spot light structure
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	GLuint uniformSpotLightCount;


	struct {
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];


	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* tessellationControlCode, const char* tessellationEvaluationCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};

