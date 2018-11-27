#include "VertexOperations.h"

//vertexDataLength - size of data for each vertex
//normalOffset - where the normal data is in vertex data

/*
//Tetrahedron example

//Index draws (order of points), so we have 12 indices and only 4 vertices because program is creating 3 vertices in each location (with same parameters)
	unsigned int indices[] = {
		0, 3, 1, //First triangle with 3 points
		1, 3, 2, //Second triangle with 3 points
		2, 3, 0, //Third triangle with 3 points
		0, 1, 2 //Fourth triangle with 3 points
	};

//Four vertices: (-1-1,0), (0,-1,1), (1,-1,0), (0,1,0) + (UV.x, UV.y) to each
	GLfloat vertices[] = {
		// x      y      z        u     v       normal vec (x,y,z)   tangent vec (x,y,z)   
		-1.0f, -1.0f,  0.0f,     0.0f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f, //Point 0 
		0.0f,  -1.0f,  1.0f,     0.5f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f, //Point 1
		1.0f,  -1.0f,  0.0f,     1.0f, 0.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f, //Point 2
		0.0f,   1.0f,  0.0f,     0.5f, 1.0f,    0.0f, 0.0f, 0.0f,	 0.0f, 0.0f, 0.0f  //Point 3
	};
*/







VertexOperations::VertexOperations()
{
}


void VertexOperations::CalcAverageNormals(std::vector<unsigned int> &indices, unsigned int indiceCount, std::vector<GLfloat> &vertices, unsigned int verticeCount, unsigned int vertexDataLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vertexDataLength;
		unsigned int in1 = indices[i + 1] * vertexDataLength;
		unsigned int in2 = indices[i + 2] * vertexDataLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v2, v1);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vertexDataLength; i++)
	{
		unsigned int nOffset = i * vertexDataLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void VertexOperations::CalculateTangents(std::vector<unsigned int> &indices, unsigned int indiceCount, std::vector<GLfloat> &vertices, unsigned int verticeCount, unsigned int vertexDataLength, unsigned int uvOffset, unsigned int tangentOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		//Set pointers to vertex position
		unsigned int in0 = indices[i] * vertexDataLength;
		unsigned int in1 = indices[i + 1] * vertexDataLength;
		unsigned int in2 = indices[i + 2] * vertexDataLength;

		glm::vec3 edge1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 edge2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		//Texture coordinates
		GLfloat deltaU1 = vertices[in1 + uvOffset] - vertices[in0 + uvOffset]; //Get U (x)
		GLfloat deltaV1 = vertices[in1 + uvOffset + 1] - vertices[in0 + uvOffset + 1]; //Get V (y)
		GLfloat deltaU2 = vertices[in2 + uvOffset] - vertices[in0 + uvOffset]; //Get U (x)
		GLfloat deltaV2 = vertices[in2 + uvOffset + 1] - vertices[in0 + uvOffset + 1]; //Get V (y)

		//Calculate tangents
		glm::vec3 tangent = glm::vec3(0, 0, 0);
		GLfloat t = (deltaU1 * deltaV2 - deltaV1 * deltaU2);
		if (t != 0)
		{
			tangent = glm::vec3((deltaV2 * edge1.x - deltaV1 * edge2.x), (deltaV2 * edge1.y - deltaV1 * edge2.y), (deltaV2 * edge1.z - deltaV1 * edge2.z)) * (1.0f / t); //Calculate trangent
		}

		//Add tangent data to vertex data
		vertices[in0 + tangentOffset] += tangent.x; vertices[in0 + tangentOffset + 1] += tangent.y; vertices[in0 + tangentOffset + 2] += tangent.z;
		vertices[in1 + tangentOffset] += tangent.x; vertices[in1 + tangentOffset + 1] += tangent.y; vertices[in1 + tangentOffset + 2] += tangent.z;
		vertices[in2 + tangentOffset] += tangent.x; vertices[in2 + tangentOffset + 1] += tangent.y; vertices[in2 + tangentOffset + 2] += tangent.z;
	}

	for (size_t i = 0; i < verticeCount / vertexDataLength; i++) //Normalize tangent vectors
	{
		unsigned int tOffset = i * vertexDataLength + tangentOffset;
		glm::vec3 vec(vertices[tOffset], vertices[tOffset + 1], vertices[tOffset + 2]);
		vec = glm::normalize(vec);
		vertices[tOffset] = vec.x; vertices[tOffset + 1] = vec.y; vertices[tOffset + 2] = vec.z;
	}
}

VertexOperations::~VertexOperations()
{
}
