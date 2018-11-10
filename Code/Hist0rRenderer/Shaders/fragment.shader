//Uniform variables are being got in shader.cpp during shader compilation process

#version 330	

in vec4 vertexColor;	
in vec2 TexCoord;
in vec3 Normal;
//flat in vec3 Normal; //OpenGL keyword to create fast flat shading (should not be used)
in vec3 FragPos;

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


uniform sampler2D theTexture; //Sampler that uses current Texture Unit
uniform Material material;

uniform vec3 cameraPosition;


vec4 CalcLightByDirection(Light light, vec3 direction)
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

	return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	vec4 colour = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
		pLight.linear * distance +
		pLight.constant;

	return (colour / attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]);
	}

	return totalColour;
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction); //Factor that indicates if fragment is in spotLight's cone

	if (slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);

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
		totalColour += CalcSpotLight(spotLights[i]);
	}

	return totalColour;
}

void main()
{
	vec4 finalColour = CalcDirectionalLight();
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	colour = texture(theTexture, TexCoord) * finalColour; //Final colour of texture affected by light
}
