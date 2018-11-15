#include "Terrain.h"

#include "stb_image.h"

Terrain::Terrain()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Terrain::Terrain(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

Mesh* Terrain::CreateTerrain()
{
	unsigned char *texDataTerrain = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (width <= 0 && height <= 0)
	{
		printf("Height map not found at %s \n", fileLocation);
		return NULL;
	}
	printf("%d, %d\n", width, height);
	printf("%d\n", texDataTerrain[0]);

	printf("Test");
	std::vector<GLfloat> verticesTerrain;
	int x = 0;
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{	
			int r = texDataTerrain[(col + row * width) * 3 + 0];
			int g = texDataTerrain[(col + row * width) * 3 + 1];
			int b = texDataTerrain[(col + row * width) * 3 + 2];
			int gValue = (r + g + b) / 3;
			float xpos = ((float)col / (float)(width - 1)) - 0.5f;
			float ypos = (float)gValue / (float)255;
			float zpos = ((float)row / (float)(height - 1)) - 0.5f;



			verticesTerrain.push_back(xpos);
			verticesTerrain.push_back(ypos);
			verticesTerrain.push_back(zpos);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			verticesTerrain.push_back(0);
			x++;
			//printf("%d\n", x);
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

	printf("Test");
	//delete terrainHeightMap;

	
//	CalcAverageNormals(&indicesTerrain[0], indicesTerrain.size(), &verticesTerrain[0], verticesTerrain.size(), 8, 5); //Calculate average normal vectors for each vertex (Phong shading)
	
	printf("ver %d, ind %d", verticesTerrain.size(), indicesTerrain.size());

	Mesh *terrain = new Mesh();
	terrain->CreateMesh(&verticesTerrain[0], &indicesTerrain[0], verticesTerrain.size(), indicesTerrain.size());
	return terrain;
	//meshList.push_back(terrain); //Add to the end of the list
}

void Terrain::CalcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
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
}
