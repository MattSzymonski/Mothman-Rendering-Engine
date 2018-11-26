#version 330
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane; //How far away we want light to reach

void main()
{
	float distance = length(FragPos.xyz - lightPos); //Distance between fragment and light source
	distance = distance/farPlane; //Normalization of the distance in relation to far plane
	gl_FragDepth = distance; 
}