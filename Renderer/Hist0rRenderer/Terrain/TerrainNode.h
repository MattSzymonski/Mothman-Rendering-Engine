#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>
#include "../VertexOperations.h"
#include "../CommonValues.h"
#include "../Camera.h"

#include "PatchVBO.h"
#include "TerrainConfig.h"

class TerrainConfig;

class TerrainNode
{
public:

	TerrainNode(TerrainConfig* config, PatchVBO* buffer, Camera* camera, glm::vec2 location, int lod, glm::vec2 index);
	void Render();
	void Update();
	
	TerrainConfig* config;
	Camera* camera;

	bool isleaf;
	GLuint lod;
	glm::vec2 location; //Bottom left corner of the quad within the quadtree
	glm::vec3 worldPos;
	glm::vec2 index; //Indicates the position of the  quad within the quad of it's parent ( (0,0) - bot left, (0,1) - bot right, (1,0) - top left, (1,1) - top right )
	GLfloat gap; //Side lenght of the quad

	glm::mat4 localMatrix;
	glm::mat4 worldMatrix;

	~TerrainNode();

private:	

	PatchVBO* buffer;
	std::vector<TerrainNode*> children;
	void AddChildNodes(int lod);
	void RemoveChildNodes();
	void ComputeWorldPos();
};

