#version 330

layout (location = 0) in vec3 pos; //Position of a vertex

uniform mat4 model;
uniform mat4 directionalLightTransform; //Point of view of the light

void main()
{
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0); //Converting position of the vertex to the coordinates from the camera point of view
}

