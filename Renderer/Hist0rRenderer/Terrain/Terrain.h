
/*
Terrain has quadtree which has 8 child nodes of lod 0

Each update:
If node is a leaf and its distance to the camera is smaller than the value of lod for its lod level it creates 4 childrens with lod level incremented by 1.
All nodes use same buffer (VBO) on GPU. Each node updates buffer with its own values before draw.

Morphing of edges on the transition between lod levels is done by vertex shader.


*/

#pragma once

#include "TerrainConfig.h"
#include "TerrainQuadtree.h"
#include "..\Camera.h"

class Terrain
{
public:

	Terrain();
	void Init(TerrainConfig* config, Camera* camera);
	void UpdateTerrain();
	void Render();

	TerrainConfig *config;
	Texture  *heightmap;
	Texture *normalTexture;

	~Terrain();
	

private:
	TerrainQuadtree *quadtree;
	
	Camera *camera;
	glm::vec3 cameraPos;
	glm::vec3 cameraForward;
};

