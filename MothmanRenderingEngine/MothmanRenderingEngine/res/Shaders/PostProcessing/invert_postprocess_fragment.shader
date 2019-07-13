#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

void main()
{
	color = vec4(vec3(1.0 - texture(s_screenTexture, v_texCoords)), 1.0);
}

