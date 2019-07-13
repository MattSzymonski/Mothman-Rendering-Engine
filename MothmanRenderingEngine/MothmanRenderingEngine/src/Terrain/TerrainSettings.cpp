#include "TerrainSettings.h"



TerrainSettings::TerrainSettings(const string& terrainName, Shader* terrainShader, Camera* camera, int rootNodeCount, GLfloat scaleXZ, GLfloat scaleY, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation)
{
	this->terrainName = terrainName;
	this->terrainShader = terrainShader;
	this->camera = camera;


	this->rootNodeCount = rootNodeCount;
	this->scaleXZ = scaleXZ;
	this->scaleY = scaleY;

	for (int i = 0; i < 8; i++)
	{
		this->lodRange[i] = lodRange[i];
		this->lodMorphingArea[i] = 0;
	}

	this->tessellationFactor = tessellationFactor;
	this->tessellationSlope = tessellationSlope;
	this->tessellationShift = tessellationShift;

	this->heightmapLocation = heightmapLocation;
	this->normalTextureLocation = normalTextureLocation;


	for (int i = 0; i < 8; i++) { // Setting morphing area
		if (this->lodRange[i] == 0)
		{
			this->lodMorphingArea[i] = 0;
		}
		else
		{
			this->lodMorphingArea[i] = this->lodRange[i] - ((int)(scaleXZ / 8.0f / glm::pow(2, i + 1)));
		}
	}

	heightmap = new Texture(this->heightmapLocation, TexType::Heightmap);
	heightmap->LoadTexture();

	normalTexture = new Texture(this->normalTextureLocation, TexType::Normal);
	normalTexture->LoadTexture();

	//Get uniform locations
	terrainUniform_cameraPosition = this->terrainShader->GetUniformLocation("u_cameraPosition");
	terrainUniform_localMatrix = this->terrainShader->GetUniformLocation("u_localMatrix");
	terrainUniform_worldMatrix = this->terrainShader->GetUniformLocation("u_worldMatrix");
	terrainUniform_scaleY = this->terrainShader->GetUniformLocation("u_scaleY");
	terrainUniform_lod = this->terrainShader->GetUniformLocation("u_lod");
	terrainUniform_index = this->terrainShader->GetUniformLocation("u_index");
	terrainUniform_gap = this->terrainShader->GetUniformLocation("u_gap");
	terrainUniform_location = this->terrainShader->GetUniformLocation("u_location");
	terrainUniform_heightmap = this->terrainShader->GetSamplerLocation("s_heightmap");
	for (size_t i = 0; i < 8; i++)
	{
		string baseName = string("u_lodMorphArea[") + to_string(i) + string("]");
		terrainUniform_morphArea[i] = this->terrainShader->GetUniformLocation(baseName);
	}

	terrainUniform_viewProjection = this->terrainShader->GetUniformLocation("u_viewProjection");
	terrainUniform_tessellationFactor = this->terrainShader->GetUniformLocation("u_tessellationFactor");
	terrainUniform_tessellationSlope = this->terrainShader->GetUniformLocation("u_tessellationSlope");
	terrainUniform_tessellationShift = this->terrainShader->GetUniformLocation("u_tessellationShift");
	terrainUniform_textureNormal = this->terrainShader->GetSamplerLocation("s_textureNormal");
}


TerrainSettings::~TerrainSettings()
{
}
