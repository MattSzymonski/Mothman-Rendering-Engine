#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

void main()
{
	color = texture(s_screenTexture, v_texCoords);
}