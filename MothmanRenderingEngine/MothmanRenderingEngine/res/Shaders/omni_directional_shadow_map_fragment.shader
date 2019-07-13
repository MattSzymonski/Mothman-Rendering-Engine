#version 330
in vec4 g_fragPos;

uniform vec3 u_lightPos;
uniform float u_farPlane; //How far away we want light to reach

void main()
{
	float distance = length(g_fragPos.xyz - u_lightPos); //Distance between fragment and light source
	distance = distance/u_farPlane; //Normalization of the distance in relation to far plane
	gl_FragDepth = distance; 
}