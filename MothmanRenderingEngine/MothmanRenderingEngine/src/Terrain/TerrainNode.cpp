#include "TerrainNode.h"

TerrainNode::TerrainNode(TerrainSettings* terrainSettings, PatchVBO* buffer, glm::vec2 location, int lod, glm::vec2 index)
{
	this->terrainSettings = terrainSettings;
	this->buffer = buffer;
	isleaf = true;
	this->index = index;
	this->lod = lod;
	this->location = location;
	this->gap = 1.0f / (GLfloat)((GLfloat)this->terrainSettings->GetRootNodeCount() * glm::pow(2, lod)); //0.125 for lod 0
	worldPos = glm::vec3(0, 0, 0);

	//Determines the position within the terrain quadtree
	//Transform vertex data on the GPU on the local transform into the right position of the terrain quadtree
	localMatrix = glm::translate(localMatrix, glm::vec3(location.x, 0, location.y));
	localMatrix = glm::scale(localMatrix, glm::vec3(gap, 0, gap));

	ComputeWorldPos();
	Update();
}

void TerrainNode::Render()
{
	if (isleaf)
	{
		glUniformMatrix4fv(terrainSettings->GetTerrainUniform_localMatrix(), 1, GL_FALSE, glm::value_ptr(localMatrix));
		glUniform1i(terrainSettings->GetTerrainUniform_lod(), lod);
		glUniform2f(terrainSettings->GetTerrainUniform_index(), index.x, index.y);
		glUniform1f(terrainSettings->GetTerrainUniform_gap(), gap);
		glUniform2f(terrainSettings->GetTerrainUniform_location(), location.x, location.y);

		buffer->Draw();
	}
	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->Render();
	}
}

void TerrainNode::Update() 
{
	//Set world y position
	if (terrainSettings->GetCamera()->GetCameraPosition().y > terrainSettings->GetScaleY())
	{
		worldPos.y = terrainSettings->GetScaleY(); //better to use worldPosition maybe??
	}
	else
	{
		worldPos.y = terrainSettings->GetCamera()->GetCameraPosition().y;
	}

	GLfloat distance = glm::length(terrainSettings->GetCamera()->GetCameraPosition() - worldPos);
	if (isleaf)
	{		
		if (distance < terrainSettings->GetLodRange()[lod])
		{
			AddChildNodes(lod + 1);
		}
	}
	else
	{
		if (distance >= terrainSettings->GetLodRange()[lod])
		{
			RemoveChildNodes();
		}
	}

	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->Update();
	}	
}

void TerrainNode::AddChildNodes(int lod)
{

	isleaf = false;
	
	if (children.size() == 0)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				children.push_back(new TerrainNode(terrainSettings, buffer, location + glm::vec2(i*gap / 2.0f, j*gap / 2.0f), lod, glm::vec2(i, j)));
			}
		}
	}
	
}

void TerrainNode::RemoveChildNodes()
{
	isleaf = true;
	if (children.size() != 0)
	{
		children.clear();
	}
}

void TerrainNode::ComputeWorldPos()
{
	glm::vec2 loc = ((location + (gap / 2.0f)) *  terrainSettings->GetScaleXZ()) - (terrainSettings->GetScaleXZ() / 2.0f);
	this->worldPos = glm::vec3(loc.x, 0, loc.y);
}

TerrainNode::~TerrainNode()
{

}
