#pragma once

#include <GL\glew.h>
class PatchVBO
{
public:

	PatchVBO();

	GLuint VBO;
	GLuint VAO;
	GLuint size;

	void Allocate(float *vertices, unsigned int verticesLength, unsigned int patchSize);
	void Draw();
	void Delete();

	~PatchVBO();
};

