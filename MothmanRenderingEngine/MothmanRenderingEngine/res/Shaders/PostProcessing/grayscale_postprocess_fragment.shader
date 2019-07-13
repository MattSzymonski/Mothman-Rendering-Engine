#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

void main()
{
	color = texture(s_screenTexture, v_texCoords);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average, average, average, 1.0);
}