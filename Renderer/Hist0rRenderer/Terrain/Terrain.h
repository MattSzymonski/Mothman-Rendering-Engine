#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>

#include "../VertexOperations.h"
#include "../CommonValues.h"

class Terrain
{
public:
	Terrain();
	~Terrain();

	Terrain(const char* terrainHeightMapLocation, const char* terrainTextureLocation, float terrainDisplacementStrength);

	void CreateTerrain();
	void RenderTerrain();

private:

	int width, height, bitDepth;

	float terrainDisplacementStrength;
	Mesh* terrainMesh;
	Texture* terrainTexture;
	const char* terrainTextureLocation;
	const char* terrainHeightMapLocation;
};

