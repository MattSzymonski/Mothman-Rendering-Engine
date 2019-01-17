#version 430

layout(vertices = 16) out;

in vec2 mapCoord_TC[];
out vec2 mapCoord_TE[];

const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

uniform int tessellationFactor;
uniform float tessellationSlope;
uniform float tessellationShift;
uniform vec3 cameraPosition;

// Calculate tessellation levels
float lodFactor(float dist) 
{
	float tessellationLevel = max(0.0, tessellationFactor/pow(dist, tessellationSlope) + tessellationShift);
	return tessellationLevel;
}

void main()
{

	if (gl_InvocationID == 0){
	
		// Calculate mid points of the edges of the quad
		vec3 abMid = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position)/2.0;	//Bottom left, Bottom right
		vec3 bcMid = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position)/2.0; //Bottom right Top right
		vec3 cdMid = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position)/2.0; //Top right, Top left
		vec3 daMid = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position)/2.0; //Top left, Bottom left
		
		// Calculate distance between camera and mid points of the edges of the quad
		float distanceAB = distance(abMid, cameraPosition);
		float distanceBC = distance(bcMid, cameraPosition);
		float distanceCD = distance(cdMid, cameraPosition);
		float distanceDA = distance(daMid, cameraPosition);
		
		// Tesselation levels used by tessellation primitive generator (define how much tessellation to apply to the patch). Value between 1 and gl_MaxTessGenLevel, depending on lodFactor.
		gl_TessLevelOuter[AB] = mix(1, gl_MaxTessGenLevel, lodFactor(distanceAB));
		gl_TessLevelOuter[BC] = mix(1, gl_MaxTessGenLevel, lodFactor(distanceBC));
		gl_TessLevelOuter[CD] = mix(1, gl_MaxTessGenLevel, lodFactor(distanceCD));
		gl_TessLevelOuter[DA] = mix(1, gl_MaxTessGenLevel, lodFactor(distanceDA));
		
		gl_TessLevelInner[0] = (gl_TessLevelOuter[BC] + gl_TessLevelOuter[DA])/4;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[AB] + gl_TessLevelOuter[CD])/4;
	}
	
	mapCoord_TE[gl_InvocationID] = mapCoord_TC[gl_InvocationID]; // Just pass to the next stage
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}