#include "Terrain.h"



Terrain::Terrain(const string& terrainName, Shader* terrainShader, Camera* camera, int rootNodeCount, GLfloat scaleXZ, GLfloat scaleY, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation)
{	
	terrainSettings = new TerrainSettings(terrainName, terrainShader, camera, rootNodeCount, scaleXZ, scaleY, lodRange, tessellationFactor, tessellationSlope, tessellationShift, heightmapLocation, normalTextureLocation);
	terrainModule = new TerrainModule(terrainSettings);
}


Terrain::~Terrain()
{
}
