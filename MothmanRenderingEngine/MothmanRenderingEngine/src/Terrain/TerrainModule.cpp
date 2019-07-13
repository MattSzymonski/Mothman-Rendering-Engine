#include "TerrainModule.h"

TerrainModule::TerrainModule(TerrainSettings* terrainSettings)
{
	this->terrainSettings = terrainSettings;

	GLfloat terrainScaleXZ = this->terrainSettings->GetScaleXZ();
	GLfloat terrainScaleY = this->terrainSettings->GetScaleY();
	worldMatrix = glm::translate(worldMatrix, glm::vec3(-terrainScaleXZ / (GLfloat)2, 0, -terrainScaleXZ / (GLfloat)2));
	worldMatrix = glm::scale(worldMatrix, glm::vec3(terrainScaleXZ, terrainScaleY, terrainScaleXZ));

	quadtree = new TerrainQuadtree(terrainSettings);
}

void TerrainModule::UpdateTerrain()
{
	if (terrainSettings->GetCamera()->GetCameraPosition() != cameraPos || terrainSettings->GetCamera()->GetForward() != cameraForward)
	{
		cameraPos = terrainSettings->GetCamera()->GetCameraPosition();
		quadtree->Update();
	}
}

void TerrainModule::Render(bool updateTerrain)
{	
	terrainSettings->GetTerrainShader()->UseShader();
	if (updateTerrain) 
	{
		UpdateTerrain();
	}
	terrainSettings->GetTerrainShader()->Validate();
	terrainSettings->GetHeightmap()->UseTexture();
	terrainSettings->GetNormalTexture()->UseTexture();
	UpdateShaderVariables();
	quadtree->Render();
}

void TerrainModule::UpdateShaderVariables() //These are variables common for every leaf so they can be set once per frame for whole terrain
{
	glUniformMatrix4fv(terrainSettings->GetTerrainUniform_worldMatrix(), 1, GL_FALSE, glm::value_ptr(worldMatrix));
	glUniform3f(terrainSettings->GetTerrainUniform_cameraPosition(), terrainSettings->GetCamera()->GetCameraPosition().x, terrainSettings->GetCamera()->GetCameraPosition().y, terrainSettings->GetCamera()->GetCameraPosition().z);
	glUniform1f(terrainSettings->GetTerrainUniform_scaleY(), terrainSettings->GetScaleY());
	glUniform1i(terrainSettings->GetTerrainUniform_heightmap(), HEIGHTMAP_TEXUNIT);

	for (int i = 0; i < 8; i++)
	{
		glUniform1i(terrainSettings->GetTerrainUniform_morphArea(i), terrainSettings->GetLodMorphingArea()[i]);
	}

	glUniformMatrix4fv(terrainSettings->GetTerrainUniform_viewProjection(), 1, GL_FALSE, glm::value_ptr(terrainSettings->GetCamera()->GetProjectionMatrix() * terrainSettings->GetCamera()->GetViewMatrix()));

	glUniform1i(terrainSettings->GetTerrainUniform_tessellationFactor(), terrainSettings->GetTessellationFactor());
	glUniform1f(terrainSettings->GetTerrainUniform_tessellationSlope(), terrainSettings->GetTessellationSlope());
	glUniform1f(terrainSettings->GetTerrainUniform_tessellationShift(), terrainSettings->GetTessellationShift());
	glUniform1i(terrainSettings->GetTerrainUniform_textureNormal(), NORMAL_TEXUNIT); //Not necessarily need to be updated for each node
}

TerrainModule::~TerrainModule()
{

}

