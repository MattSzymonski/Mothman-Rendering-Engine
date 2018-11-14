#include "Model.h"

Model::Model()
{
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++) //For each mesh in model
	{
		unsigned int materialIndex = meshToTex[i]; //Get mesh's material index

		if (materialIndex < textureList.size() && textureList[materialIndex] != NULL) //If texture for this mesh exist use it for this mesh
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene); 

	LoadMaterials(scene);
}

void Model::LoadNode(aiNode * node, const aiScene * scene)
{ 
	//One model can have separate meshes (like Unity reading mesh as multiple meshes selected when exporting in Blender)
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene); //node->mMeshes[i] - holds id of mesh in node, but actual mesh is stored in scene
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene); 
	}
}

void Model::LoadMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) //For each vertex in mesh read from file
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }); //Adding vertex coordinates to the list of vertices
		if (mesh->mTextureCoords[0]) //Check if mesh has texture/UV coordinates
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }); //Adding texture/UV coordinates to the list of vertices
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z }); //Adding normals to the list of vertices
	} //Do the same for next vertex in file

	for (size_t i = 0; i < mesh->mNumFaces; i++) //Each face consist of 3 vertices (indices)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) //For each vertex(index) in face add it to the list of indices
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);

}

void Model::LoadMaterials(const aiScene * scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) //For each material on scene (sum of materials of all loaded models)
	{
		aiMaterial* material = scene->mMaterials[i]; //Get one material

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) //Detect texture in material
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) //Get first diffuse texture and store it in path variable
			{
				int idx = std::string(path.data).rfind("\\"); //Find \ in string and set cursor there
				std::string filename = std::string(path.data).substr(idx + 1); //Get raw texture file name

				std::string texPath = std::string("Textures/") + filename; //Path to texture (All textures have to be in this folder. No relative or absolute paths are taken from the file itself)

				textureList[i] = new Texture(texPath.c_str()); //Create texture
				

				if (!textureList[i]->LoadTexture()) //Load this created texture
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i]) //If no texture found for that material use default texture
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTexture();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
}
