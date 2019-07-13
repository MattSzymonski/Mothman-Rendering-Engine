#pragma once

#include <glm\glm.hpp>
#include "..\Shader.h"
#include "..\Camera.h"
#include "..\Texture.h"

class TerrainSettings
{
public:
	TerrainSettings(const string& terrainName, Shader* terrainShader, Camera* camera, int rootNodeCount, GLfloat scaleY, GLfloat scaleXZ, GLuint lodRange[8], GLuint tessellationFactor, GLfloat tessellationSlope, GLfloat tessellationShift, const char* heightmapLocation, const char* normalTextureLocation);


	string GetTerrainName() { return terrainName; }
	int GetRootNodeCount() { return rootNodeCount; }
	void SetRootNodeCount(int value) { rootNodeCount = value; }
	GLfloat GetScaleXZ() { return scaleXZ; }
	void SetScaleXZ(GLfloat value) { scaleXZ = value; }
	GLfloat GetScaleY() { return scaleY; }
	void SetScaleY(GLfloat value) { scaleY = value; }

	GLuint *GetLodRange() { return lodRange; }
	GLuint *GetLodMorphingArea() { return lodMorphingArea; }

	GLuint GetTessellationFactor() { return tessellationFactor; };
	void SetTessellationFactor(GLuint value) { tessellationFactor = value; }
	GLfloat GetTessellationSlope() { return tessellationSlope; };
	void SetTessellationSlope(GLfloat value) { tessellationSlope = value; }
	GLfloat GetTessellationShift() { return tessellationShift; };
	void SetTessellationShift(GLfloat value) { tessellationShift = value; }

	const char* GetHeightmapLocation() { return heightmapLocation; }
	void SetHeightmapLocation(const char* heightmapLocation) { this->heightmapLocation = heightmapLocation; }
	const char* GetNormalTextureLocation() { return normalTextureLocation; }
	void SetNormalTextureLocation(const char* normalTextureLocation) { this->normalTextureLocation = normalTextureLocation; }

	Shader* GetTerrainShader() { return terrainShader; }
	Camera* GetCamera() { return camera; }

	//Unifrom variables in terrain shader
	GLuint GetTerrainUniform_cameraPosition() { return terrainUniform_cameraPosition; }
	GLuint GetTerrainUniform_localMatrix() { return terrainUniform_localMatrix; }
	GLuint GetTerrainUniform_worldMatrix() { return terrainUniform_worldMatrix; }
	GLuint GetTerrainUniform_scaleY() { return terrainUniform_scaleY; }
	GLuint GetTerrainUniform_lod() { return terrainUniform_lod; }
	GLuint GetTerrainUniform_index() { return terrainUniform_index; }
	GLuint GetTerrainUniform_gap() { return terrainUniform_gap; }
	GLuint GetTerrainUniform_location() { return terrainUniform_location; }
	GLuint GetTerrainUniform_heightmap() { return terrainUniform_heightmap; }
	GLuint GetTerrainUniform_morphArea(int i) { return terrainUniform_morphArea[i]; }
	GLuint GetTerrainUniform_viewProjection() { return terrainUniform_viewProjection; }
	GLuint GetTerrainUniform_tessellationFactor() { return terrainUniform_tessellationFactor; }
	GLuint GetTerrainUniform_tessellationSlope() { return terrainUniform_tessellationSlope; }
	GLuint GetTerrainUniform_tessellationShift() { return terrainUniform_tessellationShift; }
	GLuint GetTerrainUniform_textureNormal() { return terrainUniform_textureNormal; }

	Texture *GetHeightmap() { return heightmap; }
	Texture *GetNormalTexture() { return normalTexture; }

	~TerrainSettings();

private:
	string terrainName;
	Texture *heightmap;
	Texture *normalTexture;
	Shader *terrainShader;
	Camera *camera;

	//Settings
	int rootNodeCount;
	GLfloat scaleXZ;
	GLfloat scaleY;
	GLuint lodRange[8];
	GLuint lodMorphingArea[8];
	GLuint tessellationFactor;
	GLfloat tessellationSlope;
	GLfloat tessellationShift;
	const char* heightmapLocation;
	const char* normalTextureLocation;

	//Unifrom variables in terrain shader (for faster access than using SetUniform each frame)
	GLuint terrainUniform_cameraPosition, terrainUniform_localMatrix, terrainUniform_worldMatrix, terrainUniform_scaleY, terrainUniform_lod, terrainUniform_index, terrainUniform_gap, terrainUniform_location;
	GLuint terrainUniform_heightmap;
	GLuint terrainUniform_morphArea[8];
	GLuint terrainUniform_viewProjection;
	GLuint terrainUniform_tessellationFactor, terrainUniform_tessellationSlope, terrainUniform_tessellationShift;
	GLuint terrainUniform_textureNormal;
};

