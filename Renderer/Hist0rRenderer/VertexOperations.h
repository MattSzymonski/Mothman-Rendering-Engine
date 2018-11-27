#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>

class VertexOperations
{
public:
	VertexOperations();

	static void CalcAverageNormals(std::vector<unsigned int> &indices, unsigned int indiceCount, std::vector<GLfloat> &vertices, unsigned int verticeCount, unsigned int vertexDataLength, unsigned int normalOffset);
	static void CalculateTangents(std::vector<unsigned int> &indices, unsigned int indiceCount, std::vector<GLfloat> &vertices, unsigned int verticeCount, unsigned int vertexDataLength, unsigned int uvOffset, unsigned int tangentOffset);


	~VertexOperations();
};

