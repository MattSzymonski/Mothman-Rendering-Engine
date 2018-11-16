#include "Terrain.h"

#include "stb_image.h"

Terrain::Terrain()
{
	textureID = 0;
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
	textureID = 0;
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

	unsigned char *texDataTerrain = stbi_load(terrainHeightMapLocation, &width, &height, &bitDepth, 1);

	if (width <= 0 && height <= 0)
	{
		printf("Height map not found at %s \n", terrainHeightMapLocation);
		return;
	}



	printf("%d, %d\n", width, height);

	std::vector<GLfloat> verticesTerrain;
	int x = 0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			int heightValue = texDataTerrain[(col + row * width)];
			float xpos = ((float)col / (float)(width - 1)) - 0.5f;
			float ypos = (float)heightValue / (float)255 / (float)2 * terrainDisplacementStrength;
			float zpos = ((float)row / (float)(height - 1)) - 0.5f;

			verticesTerrain.push_back(xpos);
			verticesTerrain.push_back(ypos);
			verticesTerrain.push_back(zpos);
			verticesTerrain.push_back(xpos + 0.5f);
			verticesTerrain.push_back(zpos + 0.5f);
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

	CalcAverageNormals(&indicesTerrain[0], indicesTerrain.size(), &verticesTerrain[0], verticesTerrain.size(), 8, 5); //Calculate average normal vectors for each vertex (Phong shading)
	
	printf("ver %d, ind %d", verticesTerrain.size(), indicesTerrain.size());

	terrainTexture = new Texture(terrainTextureLocation);
	terrainTexture->LoadTexture();
	terrainMesh = new Mesh();
	terrainMesh->CreateMesh(&verticesTerrain[0], &indicesTerrain[0], verticesTerrain.size(), indicesTerrain.size());
}

void  Terrain::RenderTerrain()
{
	terrainTexture->UseTexture();
	terrainMesh->RenderMesh();
}

//vLength - size of data for each vertex
//normalOffset - where the normal data is in vertex data
void Terrain::CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) 
{
	for (size_t i = 0; i < indiceCount; i += 3) //Each 3 indices
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


Terrain::~Terrain()
{
	//delete terrainTexture;
	//delete terrainMesh;
}
