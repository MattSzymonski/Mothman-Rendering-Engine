#version 430

layout(quads, fractional_odd_spacing, cw) in;

in vec2 mapCoord_TE[];
out vec2 mapCoord_GS;

uniform sampler2D heightmap;
uniform float scaleY;

void main(){

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	
	// Compute new position for each tessellated vertex within the patch. gl_in with index 12, 0, 3, 15 are corners of the patch.
	vec4 position = ((1 - u) * (1 - v) * gl_in[12].gl_Position + u * (1 - v) * gl_in[0].gl_Position + u * v * gl_in[3].gl_Position +(1 - u) * v * gl_in[15].gl_Position);

	vec2 mapCoord = ((1 - u) * (1 - v) * mapCoord_TE[12] + u * (1 - v) * mapCoord_TE[0] + u * v * mapCoord_TE[3] +(1 - u) * v * mapCoord_TE[15]);
	
	float height = texture(heightmap, mapCoord).r;
	height *= scaleY;
	
	position.y = height;
	mapCoord_GS = mapCoord;
	
	gl_Position = position;
}
