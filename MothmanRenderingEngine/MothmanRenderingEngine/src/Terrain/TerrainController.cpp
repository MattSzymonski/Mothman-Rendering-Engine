#include "TerrainController.h"

TerrainController::TerrainController(Shader *terrainShader, Camera* camera)
{
	this->terrainShader = terrainShader;
	this->camera = camera;
}

Terrain * TerrainController::AddTerrain(const string& terrainName, int rootNodeCount, GLfloat scaleXZ, GLfloat scaleY, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation)
{
	if (terrains.find(terrainName) != terrains.end())
	{
		std::cout << "Failed adding terrain: " + terrainName + ". Terrain with this name is already exists" << std::endl;
		return nullptr;
	}

	Terrain *newTerrain = new Terrain(terrainName, terrainShader, camera, rootNodeCount, scaleXZ, scaleY, lodRange, tessellationFactor, tessellationSlope, tessellationShift, heightmapLocation, normalTextureLocation);
	terrains[terrainName] = newTerrain;
	return newTerrain;
}

void TerrainController::DeleteTerrain(const string & terrainName)
{
	std::unordered_map<string, Terrain*>::const_iterator terrain = terrains.find(terrainName);

	if (terrain == terrains.end())
	{
		std::cout << "Failed deleting terrain: " + terrainName + ". Terrain with this name not found" << std::endl;
	}
	else
	{
		delete terrain->second;
		terrains.erase(terrain);
	}
}

Terrain * TerrainController::GetTerrain(const string & terrainName)
{
	std::unordered_map<string, Terrain*>::const_iterator terrain = terrains.find(terrainName);

	if (terrains.find(terrainName) == terrains.end())
	{
		std::cout << "Failed getting terrain: " + terrainName + ". Terrain with this name not found" << std::endl;
		assert(false && "Failed getting terrain. Terrain with this name not found");
		return nullptr;
	}
	else
	{
		return terrain->second;
	}
	return nullptr;
}


TerrainController::~TerrainController()
{
}
