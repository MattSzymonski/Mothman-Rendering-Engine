//Uniform variables are being got in shader.cpp during shader compilation process

#version 330

//Taken from vertex shader automatically
in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
//flat in vec3 Normal; //OpenGL keyword to create fast flat shading (should not be used)
in vec3 FragPos;
in vec4 DirectionalLightSpacePos; //Where position of a fragment is relative to the light (light point of view)


out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;


struct Light
{
	vec3 colour;
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
uniform int pointLightCount;
uniform int spotLightCount;

//Lights
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];


uniform sampler2D theTexture; //Sampler that uses current Texture Unit (GL_TEXTURE0)
uniform sampler2D directionalShadowMap; //Shadow map Texture Unit (GL_TEXTURE1)
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];


uniform Material material;

uniform vec3 cameraPosition;

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
	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);  //Factor of light intensity calculated from angle between direction of light incidence and normal vector
	vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);  //Color of fragment after light calculation

	vec4 specularColour = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f) //If fragment is not being hit by diffuse light (angle >= 90) then it also is not being hit by specular light
	{
		vec3 fragToCamera = normalize(cameraPosition - FragPos); //Vector of direction between camera and fragment
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal))); //Vector of direction of reflected light

		float specularFactor = dot(fragToCamera, reflectedVertex); //Angle between reflected light direction and camera looking direction
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess); //Affection by shininess
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f); //Final specular colour affected by material properties 
		}
	}

	return (ambientColour + (1.0 - shadowFactor) * (diffuseColour + specularColour));
}

float CalcPointShadowFactor(PointLight light, int shadowIndex)
{
	
	vec3 fragToLight = FragPos - light.position; //Vector from frag to light source
	float currentDepth = length(fragToLight); //Distance(depth) from the light to hitpoint(fragment position)

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20; //PCF samples
	float viewDistance = length(cameraPosition - FragPos); //Distance between camera and fragment. To make sampling dynamic (make shadows smoothing depend on camera position)
	float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0; //How far from fragment in sample direction(gridSamplingDisk) we want to sample (in pixels of map)
	for (int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r; //Get distance from light source to point on shadow map in direction to frag + offset for sample
		closestDepth *= omniShadowMaps[shadowIndex].farPlane;   // Undo mapping [0;1]
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
	vec3 direction = FragPos - pLight.position; //Direction from light to fragment
	float distance = length(direction);
	direction = normalize(direction);

	float shadowFactor = CalcPointShadowFactor(pLight, shadowIndex); //Where shadow should be

	vec4 colour = CalcLightByDirection(pLight.base, direction, shadowFactor); 
	float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant; //Over distance attenuation

	return (colour / attenuation);
}

// ------------------- Directional light ------------------------

float CalcDirectionalShadowFactor(vec4 DirectionalLightSpacePos) //Directional shadows only
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; //Convert coordinates in relation to the light source to the normalized device coodinates (they will be between -1 and 1)
	projCoords = (projCoords * 0.5) + 0.5; //Convert the range to 0 and 1

	float currentDepth = projCoords.z; //Distance(depth) from the light to hitpoint(fragment position)

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction);

	float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);

	float shadow = 0.0; //Shadow value used in Percentage Close to Filtering edges smoothing method
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0); //Calculate texel size
	for (int x = -1; x <= 1; ++x) //9 samples (calculations), one for each neighbour pixel
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; //Get the value on the shadow map from the light source position (xy is point on plane that is cast orthogonally on shadow map)
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

vec4 CalcDirectionalLight(vec4 DirectionalLightSpacePos)
{
	float shadowFactor = CalcDirectionalShadowFactor(DirectionalLightSpacePos); //Calculate where shadow should be + PCF
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor); //Calculate how light affects object in areas where shadow should be
}

// --------------------- Point lights ---------------------------

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i], i);
	}

	return totalColour; //Return color, affected by all lights
}

// --------------------- Spot lights ---------------------------

vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction); //Factor that indicates if fragment is in spotLight's cone

	vec4 colour = CalcPointLight(sLight.base, shadowIndex);

	if (slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base, shadowIndex);

		return colour * (1.0f - (1.0f - slFactor)*(1.0f / (1.0f - sLight.edge))); //In cone
	}
	else
	{
		return vec4(0, 0, 0, 0); //Not in cone
	}
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for (int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i], i + pointLightCount);
	}

	return totalColour;
}

// ------------------------ Main ------------------------------

void main()
{
	vec4 finalColour = CalcDirectionalLight(DirectionalLightSpacePos);
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	colour = texture(theTexture, TexCoord) * finalColour; //Final colour of texture affected by light
}
