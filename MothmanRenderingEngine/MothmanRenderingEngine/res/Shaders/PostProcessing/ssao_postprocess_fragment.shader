#version 330

out vec4 color;

in vec2 v_texCoords;

uniform sampler2D s_screenTexture;

uniform sampler2D s_position;
uniform sampler2D s_normal;
uniform sampler2D s_texNoise;

uniform vec3 u_samples[64];
uniform mat4 u_projection;

uniform vec2 u_framebufferDimensions;

uniform int u_kernelSize;
uniform float u_kernelRadius;
uniform float u_bias;

void main()
{
	const vec2 noiseScale = u_framebufferDimensions / 2; 

	vec3 fragPos = texture(s_position, v_texCoords).xyz;
	vec3 normal = texture(s_normal, v_texCoords).rgb;
	vec3 randomVec = texture(s_texNoise, v_texCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal); //tangent-space to view-space matrix

	float occlusion = 0.0;
	for (int i = 0; i < u_kernelSize; ++i)
	{
		vec3 samplee = TBN * samples[i]; //Get sample position From tangent to view-space
		samplee = fragPos + samplee * u_kernelRadius;

		//transform sample to screen - space
		vec4 offset = vec4(samplee, 1.0);
		offset = projection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		float sampleDepth = texture(s_position, offset.xy).z;
		float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samplee.z + u_bias ? 1.0 : 0.0) * rangeCheck;
	}
	occlusion = 1.0 - (occlusion / u_kernelSize);
	color = occlusion;

	//color = texture(s_screenTexture, v_texCoords);
}
