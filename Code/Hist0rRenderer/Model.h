#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:

	void LoadNode(aiNode *node, const aiScene *scene);
	void LoadMesh(aiMesh *mesh, const aiScene *scene);
	void LoadMaterials(const aiScene *scene);

	std::vector<Mesh*> meshList; //Storing all meshes
	std::vector<Texture*> textureList; //Storing all textures
	std::vector<unsigned int> meshToTex; //Indexes - separate meshes, values - indexes of textures in texture list (used to bypass creating multiple same textures for different objects)
};

