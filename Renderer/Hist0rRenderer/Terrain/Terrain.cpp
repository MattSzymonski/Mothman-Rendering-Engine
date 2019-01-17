#include "Terrain.h"



Terrain::Terrain() 
{
	camera = NULL;
	cameraPos = glm::vec3(0, 0, 0);
}

void Terrain::Init(TerrainConfig* config, Camera* camera)
{
	this->config = config;
	this->camera = camera;
	quadtree = new TerrainQuadtree(this, this->config, this->camera);
	cameraPos = this->camera->GetCameraPosition();
	cameraForward = this->camera->GetForward();
	heightmap = new Texture(this->config->heightmapLocation, TexType::Heightmap);
	heightmap->LoadTexture();

	normalTexture = new Texture(this->config->normalTextureLocation, TexType::Normal);
	normalTexture->LoadTexture();
}

void Terrain::UpdateTerrain()
{
	if (this->camera->GetCameraPosition() != cameraPos || this->camera->GetForward() != cameraForward)
	{
		this->cameraPos = this->camera->GetCameraPosition();
		quadtree->Update();
	}
}

void Terrain::Render()
{	
	heightmap->UseTexture();
	normalTexture->UseTexture();
	quadtree->Render();
}


Terrain::~Terrain()
{
}
