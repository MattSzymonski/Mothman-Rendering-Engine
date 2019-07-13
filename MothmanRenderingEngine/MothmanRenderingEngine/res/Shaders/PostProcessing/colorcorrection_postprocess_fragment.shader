#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

uniform float u_gamma;
uniform vec4 u_tint;

void main()
{
    color = pow(texture(s_screenTexture, v_texCoords).rgb * u_tint.rgb, vec3(1.0 / u_gamma));
}