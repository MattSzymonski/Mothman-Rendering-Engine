#version 330

layout (location = 0) in vec3 pos;

out vec3 v_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
	v_texCoords = pos;
	gl_Position = u_projection * u_view * vec4(pos, 1.0);
}