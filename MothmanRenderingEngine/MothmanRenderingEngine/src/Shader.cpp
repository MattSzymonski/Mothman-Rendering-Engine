#include "Shader.h"

Shader::Shader()
{
	this->shaderName = "no shader name provided";
	shaderID = 0;
}

Shader::Shader(const string& shaderName)
{
	this->shaderName = shaderName;
	shaderID = 0;
}

//------------------------SHADER UTILS(Creating, Compilation, Validation)---------------------------
#pragma region SHADER UTILS

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader("From String", vertexCode, "From String", fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexLocation, vertexCode, fragmentLocation, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation) //For omni-directional shadow map shaders
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexLocation, vertexCode, geometryLocation, geometryCode, fragmentLocation, fragmentCode);
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

	CompileShader(vertexLocation, vertexCode, tessellationControlLocation, tessellationControlCode, tessellationEvaluationLocation, tessellationEvaluationCode, geometryLocation, geometryCode, fragmentLocation, fragmentCode);
}


void Shader::AddShader(GLuint theProgram, string location, const char* shaderCode, GLenum shaderType)
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
		LogShaderError("Error compiling shader " + location + " - " + (std::string)eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
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
		LogShaderError("Error validating shader program - " + (std::string)eLog);
		return;
	}
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		LogShaderError("Failed to read. File doesn't exist in " + (std::string)fileLocation);
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

void Shader::CompileShader(string vertexLocation, const char* vertexCode, string fragmentLocation, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		LogShaderError("Failed to create shader program");
		return;
	}

	AddShader(shaderID, vertexLocation, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentLocation, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}


void Shader::CompileShader(string vertexLocation, const char* vertexCode, string geometryLocation, const char* geometryCode, string fragmentLocation, const char* fragmentCode) //For omni-directional shadow map shaders
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		LogShaderError("Failed to create shader program");
		return;
	}

	AddShader(shaderID, vertexLocation, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryLocation, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentLocation, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(string vertexLocation, const char * vertexCode, string tessellationControlLocation, const char * tessellationControlCode, string tessellationEvaluationlLocation, const char * tessellationEvaluationCode, string geometryLocation, const char * geometryCode, string fragmentLocation, const char * fragmentCode) //For terrain
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		LogShaderError("Failed to create shader program");
		return;
	}
	//https://github.com/fynnfluegge/oreon-engine/search?p=8&q=%23version+430&unscoped_q=%23version+430
	AddShader(shaderID, vertexLocation, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, tessellationControlLocation, tessellationControlCode, GL_TESS_CONTROL_SHADER);
	AddShader(shaderID, tessellationEvaluationlLocation, tessellationEvaluationCode, GL_TESS_EVALUATION_SHADER);
	AddShader(shaderID, geometryLocation, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentLocation, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}
#pragma endregion

//-------------------------UNIFORM UTILS(Registering, Getting, Setting)-----------------------------
#pragma region UNIFORM UTILS

bool Shader::RegisterUniform(const string& uniformType, const string& uniformName)
{
	if (uniforms.find(uniformName) != uniforms.end())
	{
		LogShaderError("Failed to register uniform: " + uniformName + ". Uniform with this name is already registered in this shader");
		return false;
	}

	GLuint location = glGetUniformLocation(shaderID, uniformName.c_str());
	if (location == -1)
	{
		LogShaderError("Failed to register uniform: " + uniformName + ". No such uniform found in this shader's code");
		return false;
	}

	uniforms[uniformName] = UniformInfo(uniformType, location);
	return true;
}

GLuint Shader::GetUniformLocation(const string& uniformName)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform == uniforms.end())
	{
		LogShaderError("Failed getting uniform location: " + uniformName + ". Uniform not found");
	}
	else
	{
		return uniform->second.uniformLocation;
	}
}

string Shader::GetUniformType(const string& uniformName)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform == uniforms.end())
	{
		LogShaderError("Failed getting uniform type: " + uniformName + ". Uniform not found");
	}
	else
	{
		return uniform->second.uniformType;
	}
}

void Shader::SetUniform(const string& uniformName, int value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "int")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type int for: " + uniformName);
			return;
		}
		glUniform1i(uniform->second.uniformLocation, value);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: int, not found");
	}
}

void Shader::SetUniform(const string& uniformName, GLuint value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "int")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type int(GLuint) for: " + uniformName);
			return;
		}
		glUniform1i(uniform->second.uniformLocation, value);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: int(GLuint), not found");
	}
}

void Shader::SetUniform(const string& uniformName, float value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "float")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type float for: " + uniformName);
			return;
		}
		glUniform1f(uniform->second.uniformLocation, value);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: float, not found");
	}
}

void Shader::SetUniform(const string& uniformName, float value0, float value1)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec2")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec2 for: " + uniformName);
			return;
		}
		glUniform2f(uniform->second.uniformLocation, value0, value1);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec2, not found");
	}
}

void Shader::SetUniform(const string& uniformName, const glm::vec2& value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec2")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec2 for: " + uniformName);
			return;
		}
		glUniform2f(uniform->second.uniformLocation, value.x, value.y);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec2, not found");
	}
}

void Shader::SetUniform(const string& uniformName, float value0, float value1, float value2)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec3")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec3 for: " + uniformName);
			return;
		}
		glUniform3f(uniform->second.uniformLocation, value0, value1, value2);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec3, not found");
	}
}

void Shader::SetUniform(const string& uniformName, const glm::vec3& value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec3")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec3 for: " + uniformName);
			return;
		}
		glUniform3f(uniform->second.uniformLocation, value.x, value.y, value.z);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec3, not found");
	}
}

void Shader::SetUniform(const string& uniformName, float value0, float value1, float value2, float value3)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec4")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec4 for: " + uniformName);
			return;
		}
		glUniform4f(uniform->second.uniformLocation, value0, value1, value2, value3);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec4, not found");
	}
}

void Shader::SetUniform(const string& uniformName, const glm::vec4& value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "vec4")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type vec4 for: " + uniformName);
			return;
		}
		glUniform4f(uniform->second.uniformLocation, value.x, value.y, value.z, value.w);
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: vec4, not found");
	}
}

void Shader::SetUniform(const string& uniformName, const glm::mat4& value)
{
	std::unordered_map<string, UniformInfo>::const_iterator uniform = uniforms.find(uniformName);

	if (uniform != uniforms.end())
	{
		if (uniform->second.uniformType != "mat4")
		{
			LogShaderError("Failed setting uniform. Invalid uniform type mat4 for: " + uniformName);
			return;
		}
		glUniformMatrix4fv(uniform->second.uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
	}
	else
	{
		LogShaderError("Failed setting uniform. Uniform with name: " + uniformName + " and type: mat4, not found");
	}
}
#pragma endregion

//---------------------SAMPLER UTILS(Registering, Getting, Setting, Binding)------------------------
#pragma region SAMPLER UTILS
//In fact samplers are also uniforms and interaction with them can be made using uniform functions above,
//but in this renderer interaction with them is being made using sampler functions below which are the same as uniform functions except that they have different names for clarity

bool Shader::RegisterSampler(const string& samplerType, const string & samplerName)
{
	if (samplers.find(samplerName) != samplers.end())
	{
		LogShaderError("Failed to register sampler: " + samplerName + ". Sampler with this name is already registered in this shader");
		return false;
	}

	GLuint location = glGetUniformLocation(shaderID, samplerName.c_str());
	if (location == -1)
	{
		LogShaderError("Failed to register sampler: " + samplerName + ". No such sampler found in this shader's code");
		return false;
	}

	samplers[samplerName] = SamplerInfo(samplerType, location);
	return true;
}


GLuint Shader::GetSamplerLocation(const string& samplerName)
{
	std::unordered_map<string, SamplerInfo>::const_iterator sampler = samplers.find(samplerName);

	if (sampler == samplers.end())
	{
		LogShaderError("Failed getting sampler location: " + samplerName + ". Sampler not found");
	}
	else
	{
		return sampler->second.samplerLocation;
	}
}

string Shader::GetSamplerType(const string& samplerName)
{
	std::unordered_map<string, SamplerInfo>::const_iterator sampler = samplers.find(samplerName);

	if (sampler == samplers.end())
	{
		LogShaderError("Failed getting sampler type: " + samplerName + ". Sampler not found");
	}
	else
	{
		return sampler->second.samplerType;
	}
}

void Shader::SetSampler(const string& samplerName, int value)
{
	std::unordered_map<string, SamplerInfo>::const_iterator sampler = samplers.find(samplerName);

	if (sampler != samplers.end())
	{
		if (!(sampler->second.samplerType == "sampler2D" || sampler->second.samplerType == "samplerCube"))
		{
			LogShaderError("Failed setting sampler. Invalid sampler type int for: " + samplerName);
			return;
		}
		glUniform1i(sampler->second.samplerLocation, value);
	}
	else
	{
		LogShaderError("Failed setting sampler. Sampler with name: " + samplerName + " and type: sampler2D or samplerCube, not found");
	}
}

void Shader::BindSampler(const string& samplerName, GLuint textureUnit, GLuint textureToBind) //Bind texture to provided texture unit, then set sampler in shader to look for texture at texture unit with number textureUnit 
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	string samplerType = GetSamplerType(samplerName);
	if (samplerType == "sampler2D")
	{
		glBindTexture(GL_TEXTURE_2D, textureToBind);
	}
	if (samplerType == "samplerCube")
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureToBind);
	}
	SetSampler(samplerName, textureUnit); 
}



#pragma endregion

//---------------------OTHER------------------------
void Shader::LogShaderError(string errorMessage)
{
	cout << "Error in \"" << shaderName << "\"" << " -> " << errorMessage << endl;
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
		LogShaderError("Error linking program - " + (std::string)eLog);
		return;
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
}

Shader::~Shader()
{
	ClearShader();
}

