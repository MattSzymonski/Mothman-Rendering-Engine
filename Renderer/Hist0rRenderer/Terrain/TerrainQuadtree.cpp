#include "TerrainQuadtree.h"



TerrainQuadtree::TerrainQuadtree(Terrain* terrain, TerrainConfig* config, Camera* camera)
{
	this->terrain = terrain;
	this->config = config;
	this->camera = camera;

	buffer = new PatchVBO(); //Load vertex data to GPU only once. Every terrain node will be using the same data on the GPU
	buffer->Allocate(GenerateVertexData(), 16, 2, 16);

	for (int i = 0; i < this->config->rootNodes; i++)
	{
		for (int j = 0; j < this->config->rootNodes; j++)
		{
			TerrainNode *newChild = new TerrainNode(this->config, buffer, this->camera, glm::vec2((GLfloat)i / (GLfloat)this->config->rootNodes, (GLfloat)j / (GLfloat)this->config->rootNodes), 0, glm::vec2(i, j));
			terrainNodes.push_back(newChild);
		}
	}

	worldMatrix = glm::translate(worldMatrix, glm::vec3(-this->config->scaleXZ / (GLfloat)2, 0, -this->config->scaleXZ / (GLfloat)2));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(this->config->scaleXZ, this->config->scaleY, this->config->scaleXZ));
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
