#pragma once

//#include <GL\glew.h>
#include <glm\glm.hpp>
//#include "stb_image.h"
#include "Mesh.h"
#include "Texture.h"
#include <vector>


class Terrain
{
public:
	Terrain();
	~Terrain();

	Terrain(const char* terrainHeightMapLocation, const char* terrainTextureLocation, float terrainDisplacementStrength);

	void CreateTerrain();
	void RenderTerrain();
	void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset);

private:

	int width, height, bitDepth;

	float terrainDisplacementStrength;
	Mesh* terrainMesh;
	Texture* terrainTexture;
	const char* terrainTextureLocation;
	const char* terrainHeightMapLocation;
};

