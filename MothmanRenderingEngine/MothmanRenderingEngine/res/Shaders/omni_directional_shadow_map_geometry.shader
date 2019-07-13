#version 330
layout (triangles) in; //Take whole triangles in
layout (triangle_strip, max_vertices=18) out; //Strip of triangles removes necessity of declaring same vertices for a new triangle if they belong to the connected triangle

uniform mat4 u_lightMatrices[6]; //Six matrices for one light source, each pointing in other direction of a cube map (Projection * view)

out vec4 g_fragPos;

void main()
{
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face; //Change to which layer draw to, there are 6 layers
		for(int i = 0; i < 3; i++)
		{
			g_fragPos = gl_in[i].gl_Position; //Get position of vertices in triangle passed into shader
			gl_Position = u_lightMatrices[face] * g_fragPos; //Get fragment position in relation to Projection * view of light source in one direction
			EmitVertex(); //Draw triangle strip
		} 
		EndPrimitive(); //Stop drawing
	}
}