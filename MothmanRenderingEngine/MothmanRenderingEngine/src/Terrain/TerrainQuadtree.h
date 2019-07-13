#pragma once

#include <vector>

#include <glm\glm.hpp>

#include "PatchVBO.h"
#include "../Mesh.h"
#include "../Texture.h"
#include "../VertexOperations.h"
#include "../CommonValues.h"
#include "TerrainSettings.h"
#include "TerrainNode.h"

class TerrainQuadtree
{
public:

	TerrainQuadtree(TerrainSettings* terrainSettings);

	void Update();
	void Render();

	~TerrainQuadtree();
	
private:
	
	TerrainSettings* terrainSettings;
	PatchVBO* buffer;
	GLfloat* GenerateVertexData();
	std::vector<TerrainNode*> terrainNodes;

	//glm::vec3 TRANSFORMworldScale;
	//glm::vec3 TRANSFORMworldPosition;
	//glm::vec3 TRANSFORMlocalScale; //? 
	//glm::vec3 TRANSFORMlocalPosition; //?

};

