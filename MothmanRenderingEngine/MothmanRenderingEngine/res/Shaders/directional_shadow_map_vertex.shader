#version 330

layout (location = 0) in vec3 pos; //Position of a vertex

uniform mat4 u_model;
uniform mat4 u_directionalLightTransform; //Point of view of the light

void main()
{
	gl_Position = u_directionalLightTransform * u_model * vec4(pos, 1.0); //Converting position of the vertex to the coordinates from the camera point of view
}

