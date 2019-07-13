#pragma once
class TerrainShader
{
public:
	TerrainShader();

	void CreateFromFiles(const char* vertexLocation, const char* tessellationControlLocation, const char* tessellationEvaluationLocation, const char* geometryLocation, const char* fragmentLocation); //For Terrain

	void Validate();

	~TerrainShader();
};

