#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>
#include "../VertexOperations.h"
#include "../CommonValues.h"
#include "../Camera.h"

#include "PatchVBO.h"
#include "TerrainSettings.h"

class TerrainNode
{
public:
	TerrainNode(TerrainSettings* terrainSettings, PatchVBO* buffer, glm::vec2 location, int lod, glm::vec2 index);
	void Render();
	void Update();
	~TerrainNode();

private:	
	TerrainSettings* terrainSettings;
	PatchVBO* buffer;
	std::vector<TerrainNode*> children;

	bool isleaf;
	GLuint lod;
	glm::vec2 location; //Bottom left corner of the quad within the quadtree
	glm::vec3 worldPos;
	glm::vec2 index; //Indicates the position of the  quad within the quad of it's parent ( (0,0) - bot left, (0,1) - bot right, (1,0) - top left, (1,1) - top right )
	GLfloat gap; //Side lenght of the quad
	glm::mat4 localMatrix;

	void AddChildNodes(int lod);
	void RemoveChildNodes();
	void ComputeWorldPos();
};

