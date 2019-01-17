#version 430

layout (location = 0) out vec4 outputColor;

in vec2 mapCoord_FS;

uniform sampler2D textureNormal;
const vec3 lightDirection = vec3(-0.2, -1.0, -0.2);
const float intensity = 1.2;

float diffuse(vec3 direction, vec3 normal, float intensity) {

	return max(0.01, dot(normal, -direction) * intensity);
}


void main() {

	vec3 normal = texture(textureNormal, mapCoord_FS).rgb;
	float diff = diffuse(lightDirection, normal, intensity);
	vec3 rgb = vec3(0.8, 0.8, 0.8) * diff;
	
	outputColor = vec4(rgb, 1.0);
}