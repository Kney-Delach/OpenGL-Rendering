/***************************************************************************
 * Filename		: LightingFragment.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Fragment shader for Phong & Blinn-Phong lighting exploration.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#version 330

// ------------------ Fragment Output ------------------ //

layout(location = 0) out vec4 color;

// ------------------ Data Structures for Materials ------------------ //

struct Material 
{
	float Shininess;
	sampler2D TextureDiffuse;
	sampler2D TextureSpecular;
	sampler2D TextureEmission;
};

// ------------------ Light Data Structures ------------------ //

////////////////////////////////////////////////////
// POINT LIGHT /////////////////////////////////////
////////////////////////////////////////////////////
struct PointLight
{
	vec3 Ambient;
	float AttenuationConstant;
	vec3 Diffuse;
	float AttenuationLinear;
	vec3 Specular;
	float AttenuationQuadratic;
	vec3 Position;
	float _BorderPadding; // padding
};

//////////////////////////////////////////////////////////
// DIRECTIONAL LIGHT /////////////////////////////////////
//////////////////////////////////////////////////////////
struct DirectionalLight
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Direction;
	float _BorderPadding; // padding
};

//////////////////////////////////////////////////////////
// SPOT LIGHT ////////////////////////////////////////////
//////////////////////////////////////////////////////////
struct SpotLight
{
	vec3 Ambient;
	float CutoffInner;
	vec3 Diffuse;
	float CutoffOuter;
	vec3 Specular;
	float AttenuationConstant;
	vec3 Position;
	float AttenuationLinear;
	vec3 Direction;
	float AttenuationQuadratic;
};

// ------------------ Buffer Layouts ------------------ //

#define NUMBER_OF_POINT_LIGHTS 1 //todo: change this when adding more point lights
#define NUMBER_OF_SPOT_LIGHTS 1 //todo: change this when adding more spot lights

layout (std140) uniform Light_Uniforms
{
	PointLight PointLights[NUMBER_OF_POINT_LIGHTS];
	DirectionalLight DirectionalLights;
	SpotLight SpotLights[NUMBER_OF_SPOT_LIGHTS];
};

layout (std140) uniform Camera_Matrices 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
	vec3 CameraPosition;
	float _BoundaryPadding;
};

// ------------------ Input from previous shader stages ------------------ //

in ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
	mat3 v_TBN;
} IN;

uniform sampler2D u_NormalMap; // move to material
uniform Material u_Material; 
uniform bool u_BlinnPhong;
uniform bool u_ActivateEmission;
uniform bool u_EmissionTransform;
uniform float u_Time; // glfwGetTime()

// ------------------ Main Shader Routines ------------------ //

//////////////////////////////////////////////////////////
// Diffuse Multiplier ////////////////////////////////////
//////////////////////////////////////////////////////////
float Pi = 3.14159265;
float CalculateDiffuseMultiplier(vec3 normal, vec3 lightDirection)
{
	return max(dot(normal, lightDirection), 0.0) / Pi; // divide by pi to conserve energy (thus staying consistent with specular calculation approach)
}

//////////////////////////////////////////////////////////
// Specular Multiplier ///////////////////////////////////
//////////////////////////////////////////////////////////
// Energy Conservation article towards solving relative brightness problems: http://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/

float CalculateSpecularMultiplier(vec3 lightDirection, vec3 normal, vec3 viewDirection)
{
	if (u_BlinnPhong) // Blinn-Phong 
	{
		float EnergyConservation = (8.0 + u_Material.Shininess) / (8.0 * Pi);
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
		return EnergyConservation * pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess);
	}
	else // Normal Phong 
	{
		float EnergyConservation = (2.0 + u_Material.Shininess) / (2.0 * Pi);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		return EnergyConservation * pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.Shininess);
	}
}

//////////////////////////////////////////////////////////
// Attenuation Calculator ////////////////////////////////
//////////////////////////////////////////////////////////
vec3 CalculateAttenuationResults(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 fragPosition, float constant, float lin, float quadratic)
{
	float distance = length(position - fragPosition);
	float attenuation = 1.0 / (constant + lin * distance + quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

//////////////////////////////////////////////////////////
// DIRECTIONAL LIGHT CALCULATION /////////////////////////
//////////////////////////////////////////////////////////
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.Direction);

	// diffuse
	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

	// specular 
	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection);

	// combine results 
	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

	return (ambient + diffuse + specular);
}


//////////////////////////////////////////////////////////
// POINT LIGHT CALCULATION ///////////////////////////////
//////////////////////////////////////////////////////////
vec3 CalculatePointLights(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.Position - fragPosition);

	// diffuse shading
	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

	// specular shading
	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection); 

	// combine results
	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

	// attenuation
	return (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic));
}
//////////////////////////////////////////////////////////
// SPOT LIGHT CALCULATION ////////////////////////////////
//////////////////////////////////////////////////////////
vec3 CalculateSpotLights(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
	vec3 lightDirection = normalize(light.Position - fragPosition);

	// diffuse shading
	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

	// specular shading
	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection); 

	// spotlight intensity
	float theta = dot(lightDirection, normalize(-light.Direction));
	float epsilon = light.CutoffInner - light.CutoffOuter;
	float intensity = clamp((theta - light.CutoffOuter) / epsilon, 0.0, 1.0);

	// combine results
	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

	// Emission 
	vec3 emission = vec3(0);

	if (u_ActivateEmission && texture(u_Material.TextureSpecular, IN.v_TexCoord).r == 0)
	{
		emission = diff * texture(u_Material.TextureEmission, IN.v_TexCoord).rgb;
		if (u_EmissionTransform)
		{
			emission = texture(u_Material.TextureEmission, IN.v_TexCoord + vec2(0.0, u_Time)).rgb; // translate over time
			emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0; // fading 
		}
	}
	// attenuation
	return intensity * (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic) + emission);
}

/////////////////////////////////////////////////////////////////////////
///////////////// Main //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void main()
{
	// Performing all lighting calculations in world space
	//vec3 normal = normalize(IN.v_Normal);

	vec3 normal = texture(u_NormalMap, IN.v_TexCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal = normalize(IN.v_TBN * IN.v_Normal);


	vec3 viewDirection = normalize(CameraPosition - IN.v_FragPosition);

	// Directional light calculation
	vec3 result = CalculateDirectionalLight(DirectionalLights, normal, viewDirection);

	// Point Light Calculations
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
		result += CalculatePointLights(PointLights[i], normal, IN.v_FragPosition, viewDirection);

	// Spot Light Calculations
	for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)
		result += CalculateSpotLights(SpotLights[i], normal, IN.v_FragPosition, viewDirection);
		
	color = vec4(result, 1.0);
}