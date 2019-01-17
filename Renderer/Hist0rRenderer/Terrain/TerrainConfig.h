#pragma once
#include <glm\glm.hpp>
#include "..\Shader.h"

class Shader;

class TerrainConfig
{
public:
	
	// Values
	int rootNodes;

	GLfloat scaleY;
	GLfloat scaleXZ;

	GLuint lodRange[8];
	GLuint lodMorphingArea[8];

	GLuint tessellationFactor;
	GLfloat tessellationSlope;
	GLfloat tessellationShift;

	const char* heightmapLocation;
	const char* normalTextureLocation;

	// Other
	TerrainConfig();
	Shader* terrainShader;
	int SetMorphingArea(int lod);
	void SetLodRange(int index, GLuint lod_range);
	~TerrainConfig();
};

