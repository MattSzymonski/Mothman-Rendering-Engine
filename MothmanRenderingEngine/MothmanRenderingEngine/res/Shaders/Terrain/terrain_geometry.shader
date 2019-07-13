#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 te_mapCoord[];
out vec2 g_mapCoord;

uniform mat4 u_viewProjection;

void main() {	

	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 position = gl_in[i].gl_Position;
		gl_Position = u_viewProjection * position;
		g_mapCoord = te_mapCoord[i];
		EmitVertex();
	}
	
	EndPrimitive();
}