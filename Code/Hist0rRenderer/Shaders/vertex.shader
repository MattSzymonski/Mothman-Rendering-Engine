#version 330

//Passed into shader
layout (location = 0) in vec3 pos; //Position of vertex (glVertexAttribPointer in mesh)
layout (location = 1) in vec2 tex; 
layout (location = 2) in vec3 norm; //Normal of the vertex (Each vertex has one normal which is average of all neighbour surfaces) 

//Passed out from shader
out vec4 vertexColor;	
out vec2 TexCoord;
out vec3 Normal; 
//flat out vec3 Normal; //OpenGL keyword to create fast flat shading (set it in both vertex and fragment shaders) (should not be used)
out vec3 FragPos; 

//Set from code
uniform mat4 model;	
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex; //Just passing texture out

	Normal = mat3(transpose(inverse(model))) * norm; //Prevent changing direction of normals when mesh is scaled

	FragPos = (model * vec4(pos, 1.0)).xyz; //Position of place that is being lit (interpolated)
}