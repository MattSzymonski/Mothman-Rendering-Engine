/*

Class dependencies in terrain system
(Eg. TerrainController includes Terrain)
+----------------------------------------+
|                                        |
|           TerrainController            |
|                  ^                     |
|                  |                     |
|                  |                     |
|       +-----> Terrain <------+         |
|       |                      |         |
|       |                      |         |
| TerrainSettings ------> TerrainModule  |
|       |    |                 ^         |
|       |    |                 |         |
|       |    +---------> TerrainQuadTree |
|       |                      ^         |
|       |                      |         |
|       +--------------->  TerrainNode   |
|                                        |
+----------------------------------------+

Possible optimization - storing pointers to TerrainSettings variables locally in TerrainModule and TerrainNode than getting them each frame using terrainSettings->GetVariable()

Terrain shader pipeline: vertex, tessellation control, tessellation evaluation, geometry, fragment
*/


#pragma once
#include <string.h>
#include "Terrain.h"

class TerrainController
{
public:
	TerrainController() {}
	TerrainController(Shader *terrainShader, Camera* camera);
	Terrain* AddTerrain(const string& terrainName, int rootNodeCount, GLfloat scaleXZ, GLfloat scaleY, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation);
	void DeleteTerrain(const string& terrainName);
	Terrain* GetTerrain(const string& terrainName);

	~TerrainController();

private:
	std::unordered_map<string, Terrain*> terrains;
	Shader* terrainShader;
	Camera* camera;

};

