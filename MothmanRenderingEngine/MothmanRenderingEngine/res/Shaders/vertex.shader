#version 330

//Passed into shader
layout (location = 0) in vec3 pos; //Position of vertex (glVertexAttribPointer in mesh)
layout (location = 1) in vec2 tex; 
layout (location = 2) in vec3 norm; //Normal of the vertex (Each vertex has one normal which is average of all neighbour surfaces) 
layout (location = 3) in vec3 tang;

//Passed out from shader to fragment shader
out vec4 v_vertexColor;	
out vec2 v_texCoords;
out vec3 v_normal; 
//flat out vec3 v_normal; //OpenGL keyword to create fast flat shading (set it in both vertex and fragment shaders) (should not be used)
out vec3 v_fragPos; 
out vec4 v_directionalLightSpacePos; //Where position of a fragment is relative to the light (light point of u_view)
out mat3 v_TBN;

//Set from code
uniform mat4 u_model;	
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_directionalLightTransform; //Point of view of the light



void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
	v_directionalLightSpacePos = u_directionalLightTransform * u_model * vec4(pos, 1.0); //Point of u_view of the light

	v_vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);

	v_texCoords = tex; //Just passing texture cord out

	v_normal = mat3(transpose(inverse(u_model))) * norm; //Prevent changing direction of normals when mesh is scaled

	v_fragPos = (u_model * vec4(pos, 1.0)).xyz; //Position of place that is being lit (interpolated)

	//For normal mapping
	vec3 tangCamSpace = normalize(vec3(u_model * vec4(tang, 0.0))); //Tangent in camera space
	vec3 normCamSpace = normalize(vec3(u_model * vec4(norm, 0.0))); //Normal in camera space
	vec3 biTangCamSpace = normalize(cross(normCamSpace, tangCamSpace)); //Bitangent in camera space
	v_TBN = mat3(tangCamSpace, biTangCamSpace, normCamSpace);

}