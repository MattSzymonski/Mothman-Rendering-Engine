#include "TerrainNode2.h"


TerrainNode2::TerrainNode2(PatchVBO buffer, TerrainConfig config, glm::vec2 location, int lod, glm::vec2 index) : TerrainNode()
{
	this->buffer = buffer;
	this->isleaf = true;
	this->index = index;
	this->lod = lod;
	this->location = location;
	this->config = config;

	TerrainQuadtree *tq = dynamic_cast<TerrainQuadtree*>(parent);
	if (tq) 
	{ 
		this->gap = 1 / (tq->rootNodes * glm::pow(2, lod));
	}
	else
	{
		printf("Error readin root nodes while creating TerrainNode2");
		this->gap = 1;
	}
	

	this->localScale = glm::vec3(gap, 0, gap);
	this->localTranslation = glm::vec3(location.x, 0, location.y);


	ComputeWorldPos();
	UpdateQuadtree();
}

void TerrainNode2::Render()
{
	if (isleaf)
	{
		//TODO
	}

}

void TerrainNode2::UpdateQuadtree()
{
	if (cameraPosition.y > config.scaleY)
	{
		worldPos.y = config.scaleY;
	}
	else
	{
		worldPos.y = cameraPosition.y;
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->UpdateQuadtree();
	}
}

void TerrainNode2::UpdateChildNodes()
{
	GLfloat distance = glm::length(cameraPosition -  glm::vec3(worldPos.x,0,worldPos.y));

	if (distance < config.lodRange[lod])
	{
		AddChildNodes(lod + 1);
	}
	else if (distance >= config.lodRange[lod])
	{
		RemoveChildNodes();
	}

}

void TerrainNode2::AddChildNodes(int lod)
{
	if (isleaf)
	{
		isleaf = false;
	}
	if (children.size() == 0)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				AddChild(new TerrainNode2(buffer, config, glm::vec2(i*gap/2, j*gap/2), lod, glm::vec2(i,j)));
			}
		}
	}

}

void TerrainNode2::RemoveChildNodes()
{
	if (!isleaf)
	{
		isleaf = true;
	}
	if (children.size() != 0)
	{
		children.clear();
	}


}

void TerrainNode2::ComputeWorldPos()
{
	glm::vec2 loc = location + (gap / 2) * (config.scaleXZ - (config.scaleXZ / 2));

	this->worldPos = glm::vec3(loc.x, 0, loc.y);
}







TerrainNode2::~TerrainNode2()
{
}
