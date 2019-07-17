#version 330

layout (location = 0) in vec3 pos; //Position of vertex (glVertexAttribPointer in mesh)

uniform mat4 u_model;	
uniform mat4 u_projection;
uniform mat4 u_view;

void main() 
{
    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0);
}													


