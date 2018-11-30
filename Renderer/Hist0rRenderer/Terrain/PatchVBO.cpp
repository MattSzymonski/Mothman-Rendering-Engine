#include "PatchVBO.h"



PatchVBO::PatchVBO()
{

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
}

void PatchVBO::Allocate(float *vertices, unsigned int vertexLength, unsigned int patchSize)
{
	glBindVertexArray(VAO); //Make VAO active, Associate the set of vertex array data with individual allocated objects
	
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Associating created IBO identifier with IBO (GL_ELEMENT_ARRAY_BUFFER - type of IBO that we use, IBO - is buffer identifier)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * patchSize, vertices, GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0])*vertexLength, 0); //Takes 3 first values of vertices then skips 5 and takes another 3, to the end (position coordinates)
	glPatchParameteri(GL_PATCH_VERTICES, patchSize);

	glBindVertexArray(0);
}

void PatchVBO::Draw()
{
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_PATCHES, 0, size);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

}

void PatchVBO::Delete()
{
	

}


PatchVBO::~PatchVBO()
{
	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);

	glBindVertexArray(0);
}
