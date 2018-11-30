#include "TerrainNode.h"

//https://www.youtube.com/watch?v=Hi190Mjirk0&t=1s

TerrainNode::TerrainNode()
{
	worldScale = glm::vec3(0, 0, 0);
	worldPosition = glm::vec3(0, 0, 0);
	localPosition = glm::vec3(0, 0, 0);
	children = std::vector<TerrainNode*>();
	cameraPosition = glm::vec3(0, 0, 0);
}

void TerrainNode::AddChild(TerrainNode* newChild)
{
	newChild->SetParent(this);
	children.push_back(newChild);

}

void TerrainNode::SetParent(TerrainNode* newParent)
{
	parent = newParent;
}

void TerrainNode::Update()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Update();
	}
}

void TerrainNode::Input()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Input();
	}
}

void TerrainNode::UpdateQuadtree()
{
}

void TerrainNode::Render()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Render();
	}
}




TerrainNode::~TerrainNode()
{
}
