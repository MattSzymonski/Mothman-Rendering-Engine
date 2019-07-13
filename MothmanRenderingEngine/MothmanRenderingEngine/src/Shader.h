/*
Performance note:
Terrain system which sets 22 uniforms 200 times per frame using unordered_map does it in 12 FPS (finding proper uniform by name in unordered_map) 
Terrain system which sets 22 uniforms 200 times per frame without finding them does it in 350 FPS (simply referencing to the variable, each uniform variable needs to have own variable in renderer)
*/
#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"
#include "Lighting/DirectionalLight.h"
#include "Lighting/PointLight.h"
#include "Lighting/SpotLight.h"


using namespace std;

//Structure storing type and location of the shader uniform. Needed for registering and setting uniforms
struct UniformInfo 
{
	UniformInfo() {}
	UniformInfo(const string& type, GLuint location) : uniformType(type), uniformLocation(location) {}
	
	GLuint uniformLocation = 0;
	string uniformType;
};

//Structure storing type and location of the shader samper. Needed for registering and setting samplers
struct SamplerInfo
{
	SamplerInfo() {}
	SamplerInfo(const string& type, GLuint location) : samplerType(type), samplerLocation(location) {}

	GLuint samplerLocation = 0;
	string samplerType;
};

class TerrainNode;
class TerrainConfig;


class Shader
{
public:
	Shader();
	Shader(const string& shaderName);
	
	std::string ReadFile(const char* fileLocation);
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* tessellationControlLocation, const char* tessellationEvaluationLocation, const char* geometryLocation, const char* fragmentLocation); //For Terrain

	void Validate();
	void UseShader();
	void ClearShader();

	//Uniforms
	bool RegisterUniform(const string& uniformType, const string& uniformName);
	GLuint GetUniformLocation(const string& uniformName);
	string GetUniformType(const string& uniformName);
	
	void SetUniform(const string& uniformName, int value); //int
	void SetUniform(const string& uniformName, GLuint value); //int
	void SetUniform(const string& uniformName, float value); //float
	void SetUniform(const string& uniformName, float value0, float value1); //vec2
	void SetUniform(const string& uniformName, const glm::vec2& value); //vec2
	void SetUniform(const string& uniformName, float value0, float value1, float value2); //vec3
	void SetUniform(const string& uniformName, const glm::vec3& val); //vec3
	void SetUniform(const string& uniformName, float value0, float value1, float value2, float value3); //vec4
	void SetUniform(const string& uniformName, const glm::vec4& val); //vec4
	void SetUniform(const string& uniformName, const glm::mat4& val); //mat4

	//Samplers
	bool RegisterSampler(const string& samplerType, const string& samplerName);
	GLuint GetSamplerLocation(const string& samplerName);
	string GetSamplerType(const string& samplerName);
	void SetSampler(const string& samplerName, int value); //Should not be accessible publicly
	void BindSampler(const string& samplerName, GLuint textureUnit, GLuint textureToBind);

	~Shader();

private:
	

	GLuint shaderID;
	string shaderName;

	std::unordered_map<string, UniformInfo> uniforms;
	std::unordered_map<string, SamplerInfo> samplers;



	//Terrain
	GLuint terrain_uniformCameraPosition, terrain_uniformScaleY, terrain_uniformLod, terrain_uniformIndex, terrain_uniformLocalMatrix, terrain_uniformWorldMatrix, terrain_uniformGap, terrain_uniformLocation;
	GLuint terrain_uniformMorphArea[8];
	GLuint terrain_uniformViewProjection;
	GLuint terrain_tessellationFactor, terrain_tessellationSlope, terrain_tessellationShift;
	GLuint terrain_heightmap;



	void LogShaderError(string errorMessage);
	void CompileShader(string vertexLocation, const char* vertexCode, string fragmentLocation, const char* fragmentCode);
	void CompileShader(string vertexLocation, const char* vertexCode, string geometryLocation, const char* geometryCode, string fragmentLocation, const char* fragmentCode);
	void CompileShader(string vertexLocation, const char * vertexCode, string tessellationControlLocation, const char * tessellationControlCode, string tessellationEvaluationlLocation, const char * tessellationEvaluationCode, string geometryLocation, const char * geometryCode, string fragmentLocation, const char * fragmentCode);
	void AddShader(GLuint theProgram, string location, const char* shaderCode, GLenum shaderType);

	void CompileProgram();
};

