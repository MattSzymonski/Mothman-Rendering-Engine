#include "TerrainConfig.h"

TerrainConfig::TerrainConfig()
{
	// Root nodes
	rootNodes = 8;

	// Terrain scale
	scaleY = 30;
	scaleXZ = 100;

	// Height map
	//heightmapLocation = "Textures/Terrain/TerrainHeight1.png";
	heightmapLocation = "Textures/Terrain/Terrain1_Height.bmp";
	normalTextureLocation = "Textures/Terrain/Terrain1_Normal.png";

	// Lod and morphing areas
	for (int i = 0; i < 8; i++)
	{
		lodRange[i] = 0;
		lodMorphingArea[i] = 0;
	}

	lodRange[0] = 30;
	lodRange[1] = 15;
	lodRange[2] = 5;
	lodRange[3] = 2;
	lodRange[4] = 0;
	lodRange[5] = 0;
	lodRange[6] = 0;
	lodRange[7] = 0;

	/*
	lodRange[0] = 70;
	lodRange[1] = 40;
	lodRange[2] = 25;
	lodRange[3] = 10;
	lodRange[4] = 2;
	lodRange[5] = 0;
	lodRange[6] = 0;
	lodRange[7] = 0;
	*/

	for (int i = 0; i < 8; i++) { // Setting morphing area
		if (lodRange[i] == 0)
			lodMorphingArea[i] = 0;
		else
			SetLodRange(i, lodRange[i]);	
	}

	// Tessellation
	tessellationFactor = 1;
	tessellationSlope = 0.03f;
	tessellationShift = 0.01f;
}

int TerrainConfig::SetMorphingArea(int lod)
{
	return (int)(scaleXZ / 8.0f / glm::pow(2, lod));
}

void TerrainConfig::SetLodRange(int index, GLuint lod_range)
{
	//this->lodRange[index] = lod_range;
	lodMorphingArea[index] = this->lodRange[index] - SetMorphingArea(index + 1);
}

TerrainConfig::~TerrainConfig()
{
}
