#include "TerrainQuadtree.h"

TerrainQuadtree::TerrainQuadtree(TerrainSettings* terrainSettings)
{
	this->terrainSettings = terrainSettings;

	buffer = new PatchVBO(); //Load vertex data to GPU only once. Every terrain node will be using the same data on the GPU
	buffer->Allocate(GenerateVertexData(), 16, 2, 16);


	int terrainRootNodes = this->terrainSettings->GetRootNodeCount();
	GLfloat terrainScaleXZ = this->terrainSettings->GetScaleXZ();
	GLfloat terrainScaleY = this->terrainSettings->GetScaleY();

	for (int i = 0; i < terrainRootNodes; i++)
	{
		for (int j = 0; j < terrainRootNodes; j++)
		{
			TerrainNode *newChild = new TerrainNode(this->terrainSettings, buffer, glm::vec2((GLfloat)i / (GLfloat)terrainRootNodes, (GLfloat)j / (GLfloat)terrainRootNodes), 0, glm::vec2(i, j));
			terrainNodes.push_back(newChild);
		}
	}
}


GLfloat* TerrainQuadtree::GenerateVertexData()
{
	GLfloat* verticesTerrain = new GLfloat[32];
	
	int index = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int u = 0; u < 4; u++)
		{
			if(u != 3) { verticesTerrain[index++] = 0.333f * u; } else { verticesTerrain[index++] = 1; }
			if(i != 3) { verticesTerrain[index++] = 0.333f * i; } else { verticesTerrain[index++] = 1; }
		}
	}

	return verticesTerrain;
}

void TerrainQuadtree::Update()
{
	for (unsigned int i = 0; i < terrainNodes.size(); i++)
	{
		terrainNodes[i]->Update();
	}
}

void TerrainQuadtree::Render()
{
	for (unsigned int i = 0; i < terrainNodes.size(); i++)
	{

		terrainNodes[i]->Render();
	}
}


TerrainQuadtree::~TerrainQuadtree()
{
}
