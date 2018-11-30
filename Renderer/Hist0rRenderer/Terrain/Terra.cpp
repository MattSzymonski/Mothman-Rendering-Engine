#include "Terra.h"



Terra::Terra() : TerrainNode()
{
	

}

void Terra::Init()
{
	this->config = TerrainConfig();
	AddChild(new TerrainQuadtree(config));
}


Terra::~Terra()
{
}
