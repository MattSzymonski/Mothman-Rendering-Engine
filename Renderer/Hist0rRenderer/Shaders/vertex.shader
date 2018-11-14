#version 330

//Passed into shader
layout (location = 0) in vec3 pos; //Position of vertex (glVertexAttribPointer in mesh)
layout (location = 1) in vec2 tex; 
layout (location = 2) in vec3 norm; //Normal of the vertex (Each vertex has one normal which is average of all neighbour surfaces) 

//Passed out from shader to fragment shader
out vec4 vertexColor;	
out vec2 TexCoord;
out vec3 Normal; 
//flat out vec3 Normal; //OpenGL keyword to create fast flat shading (set it in both vertex and fragment shaders) (should not be used)
out vec3 FragPos; 
out vec4 DirectionalLightSpacePos; //Where position of a fragment is relative to the light (light point of view)


//Set from code
uniform mat4 model;	
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform; //Point of view of the light



void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0); //Point of view of the light

	vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);

	TexCoord = tex; //Just passing texture out

	Normal = mat3(transpose(inverse(model))) * norm; //Prevent changing direction of normals when mesh is scaled

	FragPos = (model * vec4(pos, 1.0)).xyz; //Position of place that is being lit (interpolated)
}