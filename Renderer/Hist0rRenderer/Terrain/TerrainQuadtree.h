#pragma once

#include <glm\glm.hpp>
#include "../Mesh.h"
#include "../Texture.h"
#include <vector>

#include "TerrainNode.h"
#include "TerrainNode2.h"
#include "TerrainConfig.h"
#include "../VertexOperations.h"
#include "../CommonValues.h"
#include "PatchVBO.h"

class TerrainQuadtree : public TerrainNode
{
public:
	TerrainQuadtree(TerrainConfig config);

	GLfloat* GenerateVertexData();
	void UpdateQuadtree();
	static int rootNodes;

	~TerrainQuadtree();
	

private:
	
	PatchVBO buffer;

};

