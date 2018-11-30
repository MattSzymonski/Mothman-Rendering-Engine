#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>

#include "TerrainNode.h"
#include "TerrainConfig.h"
#include "../VertexOperations.h"
#include "../CommonValues.h"
#include "PatchVBO.h"
#include "TerrainQuadtree.h"

class TerrainNode2 : public TerrainNode
{
public:
	TerrainNode2(PatchVBO buffer, TerrainConfig config, glm::vec2 location, int lod, glm::vec2 index);
	void Render();
	void UpdateQuadtree();
	void UpdateChildNodes();
	void AddChildNodes(int lod);
	void RemoveChildNodes();
	void ComputeWorldPos();
	//render 58min

	bool isleaf;
	TerrainConfig config;
	int lod;
	glm::vec2 location;
	glm::vec2 worldPos;
	glm::vec2 index;
	float gap;
	PatchVBO buffer;

	glm::vec3 localScale;
	glm::vec3 localTranslation;

	~TerrainNode2();
private:


	

};

