#pragma once

#include <string.h>
#include "TerrainModule.h"
#include "TerrainSettings.h"
#include "..\Shader.h"


class Terrain
{
public:
	Terrain(const string& terrainName, Shader* terrainShader, Camera* camera, int rootNodeCount, GLfloat scaleXZ, GLfloat scaleY, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation);
	TerrainModule* GetTerrainModule() { return terrainModule; }
	TerrainSettings* GetTerrainSettings() { return terrainSettings; }

	~Terrain();

private:
	TerrainModule* terrainModule;
	TerrainSettings* terrainSettings;

};

