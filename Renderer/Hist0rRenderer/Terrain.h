#pragma once

//#include <GL\glew.h>
#include <glm\glm.hpp>
//#include "stb_image.h"
#include "Mesh.h"
#include <vector>

class Terrain
{
public:
	Terrain();
	~Terrain();

	Terrain(const char* fileLoc);

	Mesh* CreateTerrain();
	void CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset);

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

