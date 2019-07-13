#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

uniform float u_offset;

void main()
{
	float offsetEdge = u_offset / 300.0f;

	vec2 offsets[9] = vec2[](
		vec2(-offsetEdge, offsetEdge), // top-left
		vec2(0.0f, offsetEdge), // top-center
		vec2(offsetEdge, offsetEdge), // top-right
		vec2(-offsetEdge, 0.0f),   // center-left
		vec2(0.0f, 0.0f),   // center-center
		vec2(offsetEdge, 0.0f),   // center-right
		vec2(-offsetEdge, -offsetEdge), // bottom-left
		vec2(0.0f, -offsetEdge), // bottom-center
		vec2(offsetEdge, -offsetEdge)  // bottom-right    
		);

	float kernel[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
		);

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++)
	{
		sampleTex[i] = vec3(texture(s_screenTexture, v_texCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++)
		col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0);
}
