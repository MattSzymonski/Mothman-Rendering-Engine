#version 430

layout (location = 0) in vec2 position0;

out vec2 v_mapCoord;

uniform vec3 u_cameraPosition;
uniform mat4 u_localMatrix;
uniform mat4 u_worldMatrix;
uniform float u_scaleY;
uniform int u_lod;
uniform vec2 u_index;
uniform float u_gap;
uniform vec2 u_location;
uniform sampler2D s_heightmap;
uniform int u_lodMorphArea[8];

float morphLatitude(vec2 position)
{
	vec2 frac = position - u_location;
	
	if (u_index == vec2(0,0)){
		float morph = frac.x - frac.y;
		if (morph > 0)
			return morph;
	}
	if (u_index == vec2(1,0)){
		float morph = u_gap - frac.x - frac.y;
		if (morph > 0)
			return morph;
	}
	if (u_index == vec2(0,1)){
		float morph = frac.x + frac.y - u_gap;
		if (morph > 0)
			return -morph;
	}
	if (u_index == vec2(1,1)){
		float morph = frac.y - frac.x;
		if (morph > 0)
			return -morph;
	}
	return 0;
}

float morphLongitude(vec2 position)
{
	vec2 frac = position - u_location;
	
	if (u_index == vec2(0,0)){
		float morph = frac.y - frac.x;
		if (morph > 0)
			return -morph;
	}
	if (u_index == vec2(1,0)){
		float morph = frac.y - (u_gap - frac.x);
		if (morph > 0)
			return morph;
	}
	if (u_index == vec2(0,1)){
		float morph = u_gap - frac.y - frac.x;
		if (morph > 0)
			return -morph;
	}
	if (u_index == vec2(1,1)){
		float morph = frac.x - frac.y;
		if (morph > 0)
			return morph;
	}
	return 0;
}

vec2 morph(vec2 localPosition, int morph_area){
	
	vec2 morphing = vec2(0,0);
	
	vec2 fixPointLatitude = vec2(0,0);
	vec2 fixPointLongitude = vec2(0,0);
	float distLatitude;
	float distLongitude;
	
	// Setting latitude and longitude of the quad depending on its u_index so on which child it is
	if (u_index == vec2(0,0)) {
		fixPointLatitude = u_location + vec2(u_gap,0);
		fixPointLongitude = u_location + vec2(0,u_gap);
	}
	else if (u_index == vec2(1,0)) {
		fixPointLatitude = u_location;
		fixPointLongitude = u_location + vec2(u_gap,u_gap);
	}
	else if (u_index == vec2(0,1)) {
		fixPointLatitude = u_location + vec2(u_gap,u_gap);
		fixPointLongitude = u_location;
	}
	else if (u_index == vec2(1,1)) {
		fixPointLatitude = u_location + vec2(0,u_gap);
		fixPointLongitude = u_location + vec2(u_gap,0);
	}
		
	float planarFactor = 0;
	if(u_cameraPosition.y > abs(u_scaleY))
		planarFactor = 1;
	else
		planarFactor = u_cameraPosition.y / abs(u_scaleY);
		
	distLatitude = length(u_cameraPosition - (u_worldMatrix * vec4(fixPointLatitude.x,planarFactor,fixPointLatitude.y,1)).xyz);
	distLongitude = length(u_cameraPosition - (u_worldMatrix * vec4(fixPointLongitude.x,planarFactor,fixPointLongitude.y,1)).xyz);
					
	if (distLatitude > morph_area)
		morphing.x = morphLatitude(localPosition.xy);
	if (distLongitude > morph_area)
		morphing.y = morphLongitude(localPosition.xy);
		
	return morphing;
}

void main()
{
	vec2 localPosition = (u_localMatrix * vec4(position0.x,0,position0.y,1)).xz;
	
	// Morphing implementation(calculating new position for vertices that are in morph area/range. Without this quads will only be replaced with smaller quads. 
	if (u_lod > 0) { 
		localPosition += morph(localPosition, u_lodMorphArea[u_lod-1]); // Translate position by morphing vector 
	}
			
	float height = texture(s_heightmap, localPosition).r;
	v_mapCoord = localPosition;
			
	gl_Position = u_worldMatrix * vec4(localPosition.x, height, localPosition.y,1);
}