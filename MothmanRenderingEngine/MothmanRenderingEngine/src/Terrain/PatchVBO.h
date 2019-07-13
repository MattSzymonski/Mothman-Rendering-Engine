#pragma once

#include <GL\glew.h>
class PatchVBO
{
public:

	PatchVBO();

	GLuint VBO;
	GLuint VAO;
	GLuint size;

	void Allocate(GLfloat *vertices, unsigned int numOfVertices, unsigned int verticesLength, unsigned int patchSize);
	void Draw();

	~PatchVBO();
};

