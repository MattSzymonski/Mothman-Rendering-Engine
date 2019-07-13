#version 430

layout(quads, fractional_odd_spacing, cw) in;

in vec2 v_mapCoord[];
out vec2 te_mapCoord;

uniform sampler2D s_heightmap;
uniform float u_scaleY;

void main(){

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	
	// Compute new position for each tessellated vertex within the patch. gl_in with index 12, 0, 3, 15 are corners of the patch.
	vec4 position = ((1 - u) * (1 - v) * gl_in[12].gl_Position + u * (1 - v) * gl_in[0].gl_Position + u * v * gl_in[3].gl_Position +(1 - u) * v * gl_in[15].gl_Position);

	vec2 mapCoord = ((1 - u) * (1 - v) * v_mapCoord[12] + u * (1 - v) * v_mapCoord[0] + u * v * v_mapCoord[3] +(1 - u) * v * v_mapCoord[15]);
	
	float height = texture(s_heightmap, mapCoord).r;
	height *= u_scaleY;
	
	position.y = height;
	te_mapCoord = mapCoord;
	
	gl_Position = position;
}
