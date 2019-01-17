#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation) //For omni-directional shadow map shaders
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::CreateFromFiles(const char * vertexLocation, const char * tessellationControlLocation, const char * tessellationEvaluationLocation, const char * geometryLocation, const char * fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string tessellationControlString = ReadFile(tessellationControlLocation);
	std::string tessellationEvaluationString = ReadFile(tessellationEvaluationLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* tessellationControlCode = tessellationControlString.c_str();
	const char* tessellationEvaluationCode = tessellationEvaluationString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, tessellationControlCode, tessellationEvaluationCode, geometryCode, fragmentCode);

}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}


void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode) //For omni-directional shadow map shaders
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char * vertexCode, const char * tessellationControlCode, const char * tessellationEvaluationCode, const char * geometryCode, const char * fragmentCode) //For terrain
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}
	//https://github.com/fynnfluegge/oreon-engine/search?p=8&q=%23version+430&unscoped_q=%23version+430
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, tessellationControlCode, GL_TESS_CONTROL_SHADER);
	AddShader(shaderID, tessellationEvaluationCode, GL_TESS_EVALUATION_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();

}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID); //Link shader to GPU
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	//Get identifiers of uniform variables in shader 
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	//For each pointLight on the scene connect light variables to the shader variables
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	//For each spotLight on the scene connect light variables to the shader variables
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	//omni-light shadow variables connect to the shader variables
	for (size_t i = 0; i < MAX_SPOT_LIGHTS + MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap[i].uniformShadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].uniformFarPlane = glGetUniformLocation(shaderID, locBuff);
	}

	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	//Pointers to the texture uniform variables in shader
	uniformTextureDiffuse = glGetUniformLocation(shaderID, "textureDiffuse");
	uniformTextureNormal = glGetUniformLocation(shaderID, "textureNormal");


	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}



	//Terrain
	terrain_uniformCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");
	terrain_uniformLocalMatrix = glGetUniformLocation(shaderID, "localMatrix");
	terrain_uniformWorldMatrix = glGetUniformLocation(shaderID, "worldMatrix");
	terrain_uniformScaleY = glGetUniformLocation(shaderID, "scaleY");
	terrain_uniformLod = glGetUniformLocation(shaderID, "lod");
	terrain_uniformIndex = glGetUniformLocation(shaderID, "index");
	terrain_uniformGap = glGetUniformLocation(shaderID, "gap");
	terrain_uniformLocation = glGetUniformLocation(shaderID, "location");

	for (size_t i = 0; i < 8; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lod_morph_area[%d]", i);
		terrain_uniformMorphArea[i] = glGetUniformLocation(shaderID, locBuff);
	}

	terrain_tessellationFactor = glGetUniformLocation(shaderID, "tessellationFactor");
	terrain_tessellationSlope = glGetUniformLocation(shaderID, "tessellationSlope");
	terrain_tessellationShift = glGetUniformLocation(shaderID, "tessellationShift");

	terrain_uniformViewProjection = glGetUniformLocation(shaderID, "m_ViewProjection"); //In terrain geometry shader
	terrain_heightmap = glGetUniformLocation(shaderID, "heightmap");


}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}
GLuint Shader::GetViewLocation()
{
	return uniformView;
}
GLuint Shader::GetAmbientColourLocation()
{
	return uniformDirectionalLight.uniformColour;
}
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}
GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}
GLuint Shader::GetCameraPositionLocation()
{
	return uniformCameraPosition;
}
GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos;
}
GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount); //Set provided uniform variable in shader to value of pointLights

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);


		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i); //Activate texture unit
		glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i); //Set provided uniform variable in shader to texture unit
		glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, pLight[i].GetFarPlane()); //Set provided uniform variable in shader to farplane of this lights
	}

}

void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount); //Set provided uniform variable in shader to value of spotLights

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);


		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i); //Activate texture unit
		glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i); //Set provided uniform variable in shader to texture unit
		glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, sLight[i].GetFarPlane()); //Set provided uniform variable in shader to farplane of this lights
	}
}



void Shader::SetTextureDiffuse(GLuint textureUnit)
{
	glUniform1i(uniformTextureDiffuse, textureUnit);
}

void Shader::SetTextureNormal(GLuint textureUnit)
{
	glUniform1i(uniformTextureNormal, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);

		if (shaderType == GL_VERTEX_SHADER) { printf("Vertex - "); }
		if (shaderType == GL_TESS_CONTROL_SHADER) { printf("TessCont - "); }
		if (shaderType == GL_TESS_EVALUATION_SHADER) { printf("TessEval - "); }
		if (shaderType == GL_GEOMETRY_SHADER) { printf("Geometry - "); }
		if (shaderType == GL_FRAGMENT_SHADER) { printf("Fragment - "); }

		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
}


	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}


void Shader::UpdateTerrainUniforms(TerrainNode *node)
{

	glUniform3f(terrain_uniformCameraPosition, node->camera->GetCameraPosition().x, node->camera->GetCameraPosition().y, node->camera->GetCameraPosition().z);
	glUniformMatrix4fv(terrain_uniformViewProjection, 1, GL_FALSE, glm::value_ptr(node->camera->GetProjectionMatrix() * node->camera->GetViewMatrix())); 

	GLuint lod = node->lod;
	glm::vec2 index = node->index;
	GLfloat gap = node->gap;
	glm::vec2 location = node->location;

	glUniformMatrix4fv(terrain_uniformLocalMatrix, 1, GL_FALSE, glm::value_ptr(node->localMatrix));
	glUniformMatrix4fv(terrain_uniformWorldMatrix, 1, GL_FALSE, glm::value_ptr(node->worldMatrix));

	glUniform1f(terrain_uniformScaleY, node->config->scaleY);
	glUniform1i(terrain_uniformLod, lod);
	glUniform2f(terrain_uniformIndex, index.x, index.y);
	glUniform1f(terrain_uniformGap, gap);
	glUniform2f(terrain_uniformLocation, location.x, location.y);

	for (int i = 0; i < 8; i++)
	{
		glUniform1i(terrain_uniformMorphArea[i], node->config->lodMorphingArea[i]);
	}

	glUniform1i(terrain_tessellationFactor, node->config->tessellationFactor);
	glUniform1f(terrain_tessellationSlope, node->config->tessellationSlope);
	glUniform1f(terrain_tessellationShift, node->config->tessellationShift);

	glUniform1i(terrain_heightmap, HEIGHTMAP_TEXUNIT); //Not necessarily need to be updated for each node
	glUniform1i(uniformTextureNormal, NORMAL_TEXUNIT); //Not necessarily need to be updated for each node
}
