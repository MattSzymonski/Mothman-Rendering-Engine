#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;
uniform vec2 u_resolution;
uniform float u_radius;
uniform float u_softness;
uniform float u_intensity;

void main()
{
	color = texture2D(s_screenTexture, v_texCoords);
	vec2 position = (gl_FragCoord.xy / u_resolution.xy) - vec2(0.5);
	float len = length(position);
	float vignette = smoothstep(u_radius, u_radius - u_softness, len);
	color = mix(color, color * vignette, u_intensity);
}