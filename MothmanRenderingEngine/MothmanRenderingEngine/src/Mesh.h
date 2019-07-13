#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices); //
	void RenderMesh(); //Draw mesh to the screen
	void ClearMesh(); //Remove mesh from the GPU

	~Mesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

