#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>

#include "TerrainConfig.h"
#include "TerrainNode.h"
#include "PatchVBO.h"
#include "../VertexOperations.h"
#include "../CommonValues.h"


class Terrain;
class Camera;
class TerrainNode;
class TerrainConfig;

class TerrainQuadtree
{
public:

	TerrainQuadtree(Terrain* terrain, TerrainConfig* config, Camera* camera);

	void Update();
	void Render();

	~TerrainQuadtree();
	
private:
	
	Terrain* terrain;
	TerrainConfig* config;
	Camera* camera;
	PatchVBO* buffer;
	GLfloat* GenerateVertexData();
	std::vector<TerrainNode*> terrainNodes;

	glm::mat4 worldMatrix;
	//glm::vec3 TRANSFORMworldScale;
	//glm::vec3 TRANSFORMworldPosition;
	//glm::vec3 TRANSFORMlocalScale; //? 
	//glm::vec3 TRANSFORMlocalPosition; //?

};

