#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 tex;

out vec2 v_texCoords;

void main()
{
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
	v_texCoords = tex;
}