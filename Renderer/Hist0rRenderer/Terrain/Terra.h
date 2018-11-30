#pragma once

#include "TerrainNode.h"
#include "TerrainConfig.h"
#include "TerrainQuadtree.h"

class Terra : public TerrainNode
{
public:

	Terra();

	void Init();


	TerrainConfig config;

	//Load config

	//void AddChild(TerrainQuadtree newTerrainQuadTree);



	~Terra();
};

