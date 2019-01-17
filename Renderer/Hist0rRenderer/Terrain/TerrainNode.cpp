#include "TerrainNode.h"


TerrainNode::TerrainNode(TerrainConfig* config, PatchVBO* buffer, Camera* camera, glm::vec2 location, int lod, glm::vec2 index)
{
	this->config = config;
	this->buffer = buffer;
	this->camera = camera;
	isleaf = true;
	this->index = index;
	this->lod = lod;
	this->location = location;
	this->gap = 1.0f / (GLfloat)((GLfloat)this->config->rootNodes * glm::pow(2, lod)); //0.125 for lod 0
	worldPos = glm::vec3(0, 0, 0);

	//Determines the position within the terrain quadtree
	//Transform vertex data on the GPU on the local transform into the right position of the terrain quadtree
	localMatrix = glm::translate(localMatrix, glm::vec3(location.x, 0, location.y));
	localMatrix = glm::scale(localMatrix, glm::vec3(gap, 0, gap));

	worldMatrix = glm::translate(worldMatrix, glm::vec3(-this->config->scaleXZ / (GLfloat)2.0f, 0, -this->config->scaleXZ / (GLfloat)2.0f));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(this->config->scaleXZ, this->config->scaleY, this->config->scaleXZ));

	ComputeWorldPos();
	Update();
}

void TerrainNode::Render()
{
	if (isleaf)
	{
		config->terrainShader->UpdateTerrainUniforms(this);
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
	if (camera->GetCameraPosition().y > this->config->scaleY)
	{
		worldPos.y = this->config->scaleY; //better to use worldPosition maybe??
	}
	else
	{
		worldPos.y = camera->GetCameraPosition().y;
	}

	GLfloat distance = glm::length(camera->GetCameraPosition() - worldPos);
	if (isleaf)
	{		
		if (distance < this->config->lodRange[lod])
		{
			AddChildNodes(lod + 1);
		}
	}
	else
	{
		if (distance >= this->config->lodRange[lod])
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
				children.push_back(new TerrainNode(config, buffer, camera, location + glm::vec2(i*gap / 2.0f, j*gap / 2.0f), lod, glm::vec2(i, j)));
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
	glm::vec2 loc = ((location + (gap / 2.0f)) * config->scaleXZ) - (config->scaleXZ / 2.0f);
	this->worldPos = glm::vec3(loc.x, 0, loc.y);
}

TerrainNode::~TerrainNode()
{

}
