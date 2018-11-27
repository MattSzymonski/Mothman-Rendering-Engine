#include "Terrain.h"

#include "stb_image.h"

Terrain::Terrain()
{
	width = 0;
	height = 0;
	bitDepth = 0;
	terrainHeightMapLocation = "";
	terrainTextureLocation = "";
	terrainTexture = NULL;
	terrainMesh = NULL;
	terrainDisplacementStrength = 1;
}

Terrain::Terrain(const char* terrainHeightMapLocation, const char* terrainTextureLocation, float terrainDisplacementStrength)
{
	width = 0;
	height = 0;
	bitDepth = 0;
	this->terrainHeightMapLocation = terrainHeightMapLocation;
	this->terrainTextureLocation = terrainTextureLocation;
	terrainTexture = NULL;
	terrainMesh = NULL;
	this->terrainDisplacementStrength = terrainDisplacementStrength;
}

void Terrain::CreateTerrain()
{
	//http://nehe.gamedev.net/tutorial/beautiful_landscapes_by_means_of_height_mapping/16006/
	//https://github.com/itoral/gl-terrain-demo/blob/master/src/ter-terrain.cpp#L179
	//https://github.com/soumitrasaxena/TerrainGenerationOpenGL
	//http://www.videotutorialsrock.com/opengl_tutorial/terrain/text.php

	//https://www.google.pl/search?q=quadtree+opengl&tbm=vid&source=lnms&sa=X&ved=0ahUKEwju_KWMl9reAhXKKCwKHezGDWEQ_AUICygC&biw=1920&bih=920&dpr=1
	//https://stackoverflow.com/questions/23327282/opengl-plane-index-calculation

	//http://tulrich.com/geekstuff/sig-notes.pdf
	//http://casual-effects.blogspot.com/2014/04/fast-terrain-rendering-with-continuous.html
	//https://www.youtube.com/watch?v=Hi190Mjirk0&t=1165s

	unsigned char *texDataTerrain = stbi_load(terrainHeightMapLocation, &width, &height, &bitDepth, 1);

	if (width <= 0 && height <= 0)
	{
		printf("Height map not found at %s \n", terrainHeightMapLocation);
		return;
	}
	else
	{
		printf("Height map success w: %d, h: %d\n", width, height);;
	}


	std::vector<GLfloat> verticesTerrain;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			int heightValue = texDataTerrain[(col + row * width)];
			float xpos = ((float)col / (float)(width - 1)) - 0.5f;
			float ypos = (float)heightValue / (float)255 / (float)2 * terrainDisplacementStrength;
			float zpos = ((float)row / (float)(height - 1)) - 0.5f;

			//Position
			verticesTerrain.push_back(xpos);
			verticesTerrain.push_back(ypos);
			verticesTerrain.push_back(zpos);
			//Texture coordinates
			verticesTerrain.push_back(xpos + 0.5f);
			verticesTerrain.push_back(zpos + 0.5f);
			//Normals
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			//Tangents
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
		}
		
	}
	
	std::vector<unsigned int> indicesTerrain;
	for (int row = 0; row < height - 1; row++)
	{
		for (int col = 0; col < width - 1; col++)
		{
			int topLeftIndexNum = (row*width + col);
			int topRightIndexNum = (row*width + col + 1);
			int bottomLeftIndexNum = ((row + 1)*width + col);
			int bottomRightIndexNum = ((row + 1)*width + col + 1);

			indicesTerrain.push_back(topLeftIndexNum);
			indicesTerrain.push_back(bottomLeftIndexNum);
			indicesTerrain.push_back(topRightIndexNum);
			indicesTerrain.push_back(topRightIndexNum);
			indicesTerrain.push_back(bottomLeftIndexNum);
			indicesTerrain.push_back(bottomRightIndexNum);

		}
	}

	
	VertexOperations::CalcAverageNormals(indicesTerrain, indicesTerrain.size(), verticesTerrain, verticesTerrain.size(), 11, 5);
	VertexOperations::CalculateTangents(indicesTerrain, indicesTerrain.size(), verticesTerrain, verticesTerrain.size(), 11, 3, 8);

	printf("ver %d, ind %d\n", verticesTerrain.size() / 8, indicesTerrain.size());

	terrainTexture = new Texture(terrainTextureLocation, TexType::Diffuse);
	terrainTexture->LoadTexture();
	terrainMesh = new Mesh();
	terrainMesh->CreateMesh(&verticesTerrain[0], &indicesTerrain[0], verticesTerrain.size(), indicesTerrain.size());
}

void  Terrain::RenderTerrain()
{
	terrainTexture->UseTexture();
	terrainMesh->RenderMesh();
}


Terrain::~Terrain()
{
	//delete terrainTexture;
	//delete terrainMesh;
}
