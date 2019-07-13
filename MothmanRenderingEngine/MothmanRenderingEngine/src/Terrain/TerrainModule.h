
/*
Terrain has quadtree which has 8 child nodes of lod 0

Each update:
If node is a leaf and its distance to the camera is smaller than the value of lod for its lod level it creates 4 childrens with lod level incremented by 1.
All nodes use same buffer (VBO) on GPU. Each node updates buffer with its own values before draw.

Morphing of edges on the transition between lod levels is done by vertex shader.

*/

#pragma once

#include "..\Shader.h"
#include "..\Camera.h"
#include "TerrainQuadtree.h"
#include "TerrainSettings.h"



class TerrainModule
{
public:
	TerrainModule(TerrainSettings* terrainSettings);
	void UpdateTerrain();
	void Render(bool updateTerrain);

	~TerrainModule();	

private:
	TerrainSettings* terrainSettings;
	TerrainQuadtree *quadtree;
	void UpdateShaderVariables();
	glm::vec3 cameraPos;
	glm::vec3 cameraForward;
	glm::mat4 worldMatrix;
};

