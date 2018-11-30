#pragma once

#include <glm\glm.hpp>
#include <vector>

class TerrainNode
{

public:
	TerrainNode();

	void AddChild(TerrainNode* newChild);
	void SetParent(TerrainNode* newParent);
	void Update();
	void Render();
	void Input();

	virtual void UpdateQuadtree();

	glm::vec3 worldScale;
	glm::vec3 worldPosition;
	
	glm::vec3 localPosition;


	TerrainNode* parent;
	std::vector<TerrainNode*> children;
	glm::vec3 cameraPosition;

	~TerrainNode();

private:
	
	int test;

};

