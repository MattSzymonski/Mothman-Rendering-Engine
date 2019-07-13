#include "PatchVBO.h"

#include <stdio.h>
#include <iostream>

PatchVBO::PatchVBO()
{
	VAO = 0;
	VBO = 0;
}

void PatchVBO::Allocate(GLfloat *vertices, unsigned int numOfVertices, unsigned int vertexLength, unsigned int patchSize)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices * 2, vertices, GL_STATIC_DRAW);
	

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * vertexLength, (void*)(sizeof(vertices[0]) * 0));
	glPatchParameteri(GL_PATCH_VERTICES, patchSize);

	glBindVertexArray(0);
}

void PatchVBO::Draw()
{
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_PATCHES, 0, 16);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


PatchVBO::~PatchVBO()
{
	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glBindVertexArray(0);
}
