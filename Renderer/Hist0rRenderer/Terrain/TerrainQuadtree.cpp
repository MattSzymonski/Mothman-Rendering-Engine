#include "TerrainQuadtree.h"



TerrainQuadtree::TerrainQuadtree(TerrainConfig config) : TerrainNode()
{
	rootNodes = 8;
	
	//Load vertex data to GPU only once. Every terrain node will be using the same data on the GPU
	buffer = PatchVBO();
	buffer.Allocate(GenerateVertexData(), 2, 16);


	for (size_t i = 0; i < rootNodes; i++)
	{
		for (size_t j = 0; j < rootNodes; j++)
		{
			AddChild(new TerrainNode2(buffer, config, glm::vec2(i/rootNodes, j/rootNodes), 0, glm::vec2(i,j)));
		}
	}

	
	worldScale = glm::vec3(config.scaleXZ, config.scaleY, config.scaleXZ);
	worldPosition = glm::vec3(-config.positionXZ/2, 0, -config.positionXZ/2);
	
}


GLfloat* TerrainQuadtree::GenerateVertexData()
{
	
	GLfloat verticesTerrain[32];

	int index = 0;
	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 0;

	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 0.333;

	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 0.666;

	verticesTerrain[index++] = 0;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 0.333;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 0.666;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 1;
	verticesTerrain[index++] = 1;

	/*
	verticesTerrain[index++] = glm::vec2(0, 0); 
	verticesTerrain[index++] = glm::vec2(0.333, 0);
	verticesTerrain[index++] = glm::vec2(0.666, 0);
	verticesTerrain[index++] = glm::vec2(1, 0);

	verticesTerrain[index++] = glm::vec2(0, 0.333);
	verticesTerrain[index++] = glm::vec2(0.333, 0.333);
	verticesTerrain[index++] = glm::vec2(0.666, 0.333);
	verticesTerrain[index++] = glm::vec2(1, 0.333);

	verticesTerrain[index++] = glm::vec2(0, 0.666);
	verticesTerrain[index++] = glm::vec2(0.333, 0.666);
	verticesTerrain[index++] = glm::vec2(0.666, 0.666);
	verticesTerrain[index++] = glm::vec2(1, 0.666);

	verticesTerrain[index++] = glm::vec2(0, 1);
	verticesTerrain[index++] = glm::vec2(0.333, 1);
	verticesTerrain[index++] = glm::vec2(0.666, 1);
	verticesTerrain[index++] = glm::vec2(1, 1);
	*/


	return verticesTerrain;
}

void TerrainQuadtree::UpdateQuadtree()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->UpdateQuadtree();
	}

}


TerrainQuadtree::~TerrainQuadtree()
{
}
