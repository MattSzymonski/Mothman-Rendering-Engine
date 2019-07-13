#include "Mesh.h"

#include <iostream>

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(float *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{


	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO); //Creates one identifier for VAO and stores it in VAO variable (Allocates space for VAOs in GPU)
	glBindVertexArray(VAO); //Make VAO active, Associate the set of vertex array data with individual allocated objects



	glGenBuffers(1, &IBO); //Creates one identifier for IBO, second parameter is IBO where generated identifiers will be stored (Allocates space for VAOs in GPU)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //Associating created IBO identifier with IBO (GL_ELEMENT_ARRAY_BUFFER - type of IBO that we use, IBO - is buffer identifier)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO); //Creates one identifier for VBO, second parameter is VBO where generated identifiers will be stored (Allocates space for VAOs in GPU)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Associating created VBO identifier with VBO (GL_ARRAY_BUFFER VBO - type of VBO that we use, VBO - is buffer identifier)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); //Upload data into active buffer (GL_ARRAY_BUFFER - type of VBO that we use, GL_STATIC_DRAW - method of accessing data)

	//Passing values into vertex shader (layout (location = 0) in vec3 pos)
	//Nr, how many values to pass, , , total values for each vertex, how many skip before passing 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 0)); //Takes 3 first values of vertices then skips 11 and takes another 3, to the end (position coordinates)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 3)); //Skips 3 first values of vertices then takes 2 and skips next 11 and takes another 2, to the end (UV coordinates)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 5)); //(Normal coordinates)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 8)); //(Tangent coordinates)
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinding
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbinding

	glBindVertexArray(0); //Unbinding VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbinding (have to be called after unbinding VAO)

}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); //Associate VAO with current active buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);  //Draw by elements: 0 - first vertex, 3 - number of vertex

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh() //Deleting the buffer of GPU memory because there is no garbage collections so we need to delete this manually
{
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteBuffers(1, &VAO);
		VAO = 0;
	}
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}
