#version 330

in vec3 v_texCoords;

out vec4 color;

uniform samplerCube s_skybox;

void main()
{
	color = texture(s_skybox, v_texCoords);
}