
#version 330

out vec4 color;

in vec2 v_texCoords;
uniform float u_near;
uniform float u_far;

//uniform sampler2D s_screenTexture;
uniform sampler2D s_depthColorTexture;


float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC 
	return (2.0 * u_near * u_far) / (u_far + u_near - z * (u_far - u_near));
}

void main()
{
	//color = texture(s_screenTexture, v_texCoords);
	vec4 rawDepth = texture(s_depthColorTexture, v_texCoords);

	float depth = LinearizeDepth(rawDepth.z) / u_far;
	color = vec4(vec3(depth), 1.0);


}

