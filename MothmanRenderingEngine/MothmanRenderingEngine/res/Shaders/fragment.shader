//Uniform variables are being got in shader.cpp during shader compilation process

#version 330

//Taken from vertex shader automatically
in vec4 v_vertexColor;
in vec2 v_texCoords;
in vec3 v_normal;
//flat in vec3 v_normal; //OpenGL keyword to create fast flat shading (should not be used)
in vec3 v_fragPos;
in vec4 v_directionalLightSpacePos; //Where position of a fragment is relative to the light (light point of view)
in mat3 v_TBN;

out vec4 color;

const int MAX_POINT_LIGHTS = 5;
const int MAX_SPOT_LIGHTS = 5;
vec3 normal;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

//Light count
uniform int u_pointLightCount;
uniform int u_spotLightCount;

//Lights
uniform DirectionalLight u_directionalLight;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];


uniform sampler2D s_textureDiffuse; //Sampler that uses current Texture Unit (GL_TEXTURE1)
uniform sampler2D s_textureNormal; //Sampler that uses current Texture Unit (GL_TEXTURE2)

uniform sampler2D s_directionalShadowMap; //Shadow map Texture Unit (GL_TEXTURE1)
uniform OmniShadowMap u_omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];


uniform Material u_material;

uniform vec3 u_cameraPosition;

vec3 gridSamplingDisk[20] = vec3[] //Offset directions for 20 samples for PCF edge smoothing method in omni-directional shadows
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
	);

// ----------------------- Common -----------------------------





vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) //All shadows 
{
	vec4 ambientColour = vec4(light.color, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);  //Factor of light intensity calculated from angle between direction of light incidence and normal vector
	vec4 diffuseColour = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);  //Color of fragment after light calculation

	vec4 specularColour = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f) //If fragment is not being hit by diffuse light (angle >= 90) then it also is not being hit by specular light
	{
		vec3 fragToCamera = normalize(u_cameraPosition - v_fragPos); //Vector of direction between camera and fragment
		vec3 reflectedVertex = normalize(reflect(direction, normalize(normal))); //Vector of direction of reflected light

		float specularFactor = dot(fragToCamera, reflectedVertex); //Angle between reflected light direction and camera looking direction
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_material.shininess); //Affection by shininess
			specularColour = vec4(light.color * u_material.specularIntensity * specularFactor, 1.0f); //Final specular color affected by u_material properties 
		}
	}

	return (ambientColour + (1.0 - shadowFactor) * (diffuseColour + specularColour));
}

float CalcPointShadowFactor(PointLight light, int shadowIndex)
{

	vec3 fragToLight = v_fragPos - light.position; //Vector from frag to light source
	float currentDepth = length(fragToLight); //Distance(depth) from the light to hitpoint(fragment position)

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20; //PCF samples
	float viewDistance = length(u_cameraPosition - v_fragPos); //Distance between camera and fragment. To make sampling dynamic (make shadows smoothing depend on camera position)
	float diskRadius = (1.0 + (viewDistance / u_omniShadowMaps[shadowIndex].farPlane)) / 25.0; //How far from fragment in sample direction(gridSamplingDisk) we want to sample (in pixels of map)
	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(u_omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r; //Get distance from light source to point on shadow map in direction to frag + offset for sample
		closestDepth *= u_omniShadowMaps[shadowIndex].farPlane;   // Undo mapping [0;1]
		if (currentDepth - bias > closestDepth) // If current(distance from the light source to fragment) - bias is greather than distance from the light source to the fragment depth value on the shadow map then fragment is in shadow
		{
			shadow += 1.0;
		}
	}
	shadow /= float(samples);

	return shadow;
}

vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
	vec3 direction = v_fragPos - pLight.position; //Direction from light to fragment
	float distance = length(direction);
	direction = normalize(direction);

	float shadowFactor = CalcPointShadowFactor(pLight, shadowIndex); //Where shadow should be

	vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor);
	float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant; //Over distance attenuation

	return (color / attenuation);
}

// ------------------- Directional light ------------------------

float CalcDirectionalShadowFactor(vec4 v_directionalLightSpacePos) //Directional shadows only
{
	vec3 projCoords = v_directionalLightSpacePos.xyz / v_directionalLightSpacePos.w; //Convert coordinates in relation to the light source to the normalized device coodinates (they will be between -1 and 1)
	projCoords = (projCoords * 0.5) + 0.5; //Convert the range to 0 and 1

	float currentDepth = projCoords.z; //Distance(depth) from the light to hitpoint(fragment position)

	//vec3 normal = normalize(v_normal);
	vec3 lightDir = normalize(u_directionalLight.direction);

	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

	float shadow = 0.0; //Shadow value used in Percentage Close to Filtering edges smoothing method
	vec2 texelSize = 1.0 / textureSize(s_directionalShadowMap, 0); //Calculate texel size
	for (int x = -1; x <= 1; ++x) //9 samples (calculations), one for each neighbour pixel
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(s_directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; //Get the value on the shadow map from the light source position (xy is point on plane that is cast orthogonally on shadow map)
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0; //If current(distance from the light source to fragment) - bias is greather than distance from the light source to the fragment depth value on the shadow map then fragment is in shadow
		}
	}

	shadow /= 9.0; //Devide result by 9 because there where 9 samples (calculations)

	if (projCoords.z > 1.0) //If distance is larger than distance to far plane of the othogonal frustrum, do not draw shadows beyond that far plane
	{
		shadow = 0.0;
	}

	return shadow;
}

vec4 CalcDirectionalLight(vec4 v_directionalLightSpacePos)
{
	float shadowFactor = CalcDirectionalShadowFactor(v_directionalLightSpacePos); //Calculate where shadow should be + PCF
	return CalcLightByDirection(u_directionalLight.base, u_directionalLight.direction, shadowFactor); //Calculate how light affects object in areas where shadow should be
}

// --------------------- Point lights ---------------------------

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for (int i = 0; i < u_pointLightCount; i++)
	{
		totalColour += CalcPointLight(u_pointLights[i], i);
	}

	return totalColour; //Return color, affected by all lights
}

// --------------------- Spot lights ---------------------------

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
	vec3 rayDirection = normalize(v_fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction); //Factor that indicates if fragment is in spotLight's cone

	vec4 color = CalcPointLight(sLight.base, shadowIndex);

	if (slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base, shadowIndex);

		return color * (1.0f - (1.0f - slFactor)*(1.0f / (1.0f - sLight.edge))); //In cone
	}
	else
	{
		return vec4(0, 0, 0, 0); //Not in cone
	}
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for (int i = 0; i < u_spotLightCount; i++)
	{
		totalColour += CalcSpotLight(u_spotLights[i], i + u_pointLightCount);
	}

	return totalColour;
}

// ------------------------ Main ------------------------------

void main()
{
	//Additional normals calculation for normal mapping
	normal = texture(s_textureNormal, v_texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(v_TBN * normal);

	vec4 finalColour = CalcDirectionalLight(v_directionalLightSpacePos);
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	color = texture(s_textureDiffuse, v_texCoords) * finalColour; //Final color of texture affected by light
}
