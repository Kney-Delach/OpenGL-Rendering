/***************************************************************************
 * Filename		: MultipleLightsFragment.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Fragment shader using Phong & Blinn-Phong lighting with multiple lights.
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

layout(location = 0) out vec4 color;

struct Material 
{
	float Shininess;
	sampler2D TextureDiffuse;
	sampler2D TextureSpecular;
	sampler2D TextureEmission;
	sampler2D TextureNormal; // don't use this currently
};

// POINT LIGHT 
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


// DIRECTIONAL LIGHT 
struct DirectionalLight
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Direction;
	float _BorderPadding; // padding
};

// SPOT LIGHT 
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

#define NUMBER_OF_POINT_LIGHTS 4 
#define NUMBER_OF_SPOT_LIGHTS 4

layout (std140) uniform Light_Uniforms
{
	PointLight PointLights[NUMBER_OF_POINT_LIGHTS];
	DirectionalLight DirectionalLights;
	SpotLight SpotLights[NUMBER_OF_SPOT_LIGHTS];
};

layout (std140) uniform Camera_Uniforms 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
	vec3 CameraPosition;
	float _BoundaryPadding;
};

//todo: implement a maximum number, then choose N current lights less than the maximum
layout (std140) uniform Directional_Light_Space_Uniforms 
{
	mat4 DirectionalLightSpaceMatrix;
};

layout (std140) uniform Light_Space_Uniforms 
{
	mat4 LightSpaceMatrix[NUMBER_OF_SPOT_LIGHTS];
};

//layout (std140) uniform Point_Light_Space_Uniforms 
//{
//	//WRITE_SOME_USEFUL_STUFF_HERE_ORI!
//	//float
//};

in ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} IN;

uniform Material u_Material; 
uniform sampler2D u_PointShadowMap[NUMBER_OF_POINT_LIGHTS];
uniform sampler2D u_DirectionalShadowMap; 
uniform sampler2D u_ShadowMap[NUMBER_OF_SPOT_LIGHTS]; //todo: rename this

// Diffuse Multiplier 
float Pi = 3.14159265;
float CalculateDiffuseMultiplier(vec3 normal, vec3 lightDirection)
{
	return max(dot(normal, lightDirection), 0.0);// / Pi; // divide by pi to conserve energy
}

// Specular Multiplier with energy conservation for dynamic shininess attachment 
float CalculateSpecularMultiplier(vec3 lightDirection, vec3 normal, vec3 viewDirection)
{
	//float EnergyConservation = (8.0 + u_Material.Shininess) / (8.0 * Pi);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	return pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess); //EnergyConservation * 
}

// Attenuation Calculator
vec3 CalculateAttenuationResults(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 fragPosition, float constant, float lin, float quadratic)
{
	float distance = length(position - fragPosition);
	float attenuation = 1.0 / (constant + lin * distance + quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

float CalcDirectionalLightShadow(vec3 normal)
{
	vec4 lightSpaceFragCoord = DirectionalLightSpaceMatrix * vec4(IN.v_FragPosition, 1.0);

	 // perform perspective divide
	 vec3 projCoords = lightSpaceFragCoord.xyz / lightSpaceFragCoord.w;
	 // transform to [0,1] range
	 projCoords = projCoords * 0.5 + 0.5;

	 // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	 float closestDepth = texture(u_DirectionalShadowMap, projCoords.xy).r;

	 // get depth of current fragment from light's perspective
	 float currentDepth = projCoords.z;

//	 // check whether current frag pos is in shadow
//	 float bias = max(0.05 * (1.0 - dot(IN.v_Normal, normalize(DirectionalLights.Direction))), 0.005);
//	 float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0; // 0.005
//
	float shadow = 0;
	if (projCoords.z > 1.0)
		shadow = 0.0;
	// fixing jagged edges using PCF (percentage-closer filtering)  
	vec2 texelSize = 1.0 / textureSize(u_DirectionalShadowMap, 0);
	float bias = max(0.05 * (1.0 - dot(IN.v_Normal, normalize(DirectionalLights.Direction))), 0.005);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_DirectionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias  > pcfDepth ? 1.0 : 0.0; //0.00001
		} 
	}

	shadow /= 20.0;
	return shadow;
}

// DIRECTIONAL LIGHT CALCULATION 
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.Direction); //normalize(-light.Direction);

	// diffuse
	float diff = CalculateDiffuseMultiplier(normal, lightDirection); // correct

	// specular 
	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection);

	// shadow
	float shadow = 0.0; 
	shadow = CalcDirectionalLightShadow(normal);

	// combine results 
	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

	return (ambient + (1 - shadow) * (diffuse + specular));
}


float CalcPointLightShadow(int index)
{
	//  OUT.v_FragPosition 
//	vec3 fragToLightDirection = OUT.v_FragPosition - PointLights[index].Position; // vector between fragment position and light position
//
//	float currentDepth = length(fragToLightDirection); // translate linear depth as the length between the fragment and light position
//
//	float shadow = 0.0;
//	float bias = 0.05;
//	float samples = 4.0;
//	float offset = 0.1;
//	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
//	{
//		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
//		{
//			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
//			{
//				float closestDepth = texture(u_PointShadowMap[index], fragToLightDirection + vec3(x, y, z)).r;
//				closestDepth *= u_FarPlane; //todo: pre-set this uniform (THIS SHADER WILL NOT COMPILE UNTIL THIS IS DONE!)
//				if (currentDepth - bias > closestDepth)
//					shadow += 1.0;
//			}
//		}
//	}
//	shadow /= (samples * samples * samples);
	float shadow = 0.0;
	return shadow;
}

// POINT LIGHT CALCULATION 
vec3 CalculatePointLights(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, int index)
{
	vec3 lightDirection = normalize(light.Position - fragPosition);

	// diffuse shading
	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

	// specular shading
	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection); 

	// shadows 
	float shadow  = 0.0; 
	shadow = CalcPointLightShadow(index);
	// combine results
	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

	// attenuation
	return (1 - shadow) * (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic));
}

/////////////////////////////////////////////
//// Spot Light Shadow
/////////////////////////////////////////////

float CalcSpotLightShadow(vec3 normal, int index)
{
	//perform perspective divide
	vec4 lightSpaceFragCoord = LightSpaceMatrix[index] * vec4(IN.v_FragPosition, 1.0);
    vec3 projCoords = lightSpaceFragCoord.xyz / lightSpaceFragCoord.w;
    // transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;

	// fix for co-ordinates that are outside far plane of light's frustum.
	if(projCoords.z > 1.0) 
	{
		return 0.0;
	}
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_ShadowMap[index], projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z; 

	float shadow = 0.0;

	// fixing jagged edges using PCF (percentage-closer filtering)  
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap[index], 0);
	float bias = max(0.05 * (1.0 - dot(IN.v_Normal, normalize(-SpotLights[index].Direction))), 0.005);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_ShadowMap[index], projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias  > pcfDepth ? 1.0 : 0.0; //0.00001
		} 
	}

	shadow /= 12.0;
	return shadow;
}

// SPOT LIGHT CALCULATION 
vec3 CalculateSpotLights(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, int index)
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

	float shadow = 0.0; 
	shadow = CalcSpotLightShadow(normal, index);

	vec3 emission = vec3(0,0,0); 
	if(!(texture(u_Material.TextureEmission, IN.v_TexCoord).rgb == vec3(1,1,1)) && index == 0)
	{
		emission = texture(u_Material.TextureEmission, IN.v_TexCoord).rgb;
	}
		
	// attenuation
	return (1 - shadow) * intensity * (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic)) + emission;
}


// Main 
void main()
{	
	vec4 transparency = texture(u_Material.TextureDiffuse, IN.v_TexCoord);
	if(transparency.a  == 0)
		discard; 

	vec3 viewDirection = normalize(CameraPosition - IN.v_FragPosition);

	vec3 normal = IN.v_Normal;
	if(!(texture(u_Material.TextureNormal, IN.v_TexCoord).rgb == vec3(1,1,1)))
		normal =  normalize((texture(u_Material.TextureNormal, IN.v_TexCoord).rgb * 2.0 - 1.0));
	
	// Directional light calculation
	vec3 result = CalculateDirectionalLight(DirectionalLights, normal, viewDirection);

	// Point Light Calculations
	for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
		result += CalculatePointLights(PointLights[i], normal, IN.v_FragPosition, viewDirection, i);

	// Spot Light Calculations
	for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)
		result += CalculateSpotLights(SpotLights[i], normal, IN.v_FragPosition, viewDirection, i);
		
		color = vec4(result, transparency.a);
}

////////////////////////////////////////////////////////////////////////////////////////////////
///// Removed stuff 
////////////////////////////////////////////////////////////////////////////////////////////////

// Spot Light Extras 
	// Emission 
	//vec3 emission = vec3(0);

	// if (u_ActivateEmission && texture(u_Material.TextureSpecular, IN.v_TexCoord).r == 0)
	// {
	// 	emission = diff * texture(u_Material.TextureEmission, IN.v_TexCoord).rgb;
	// 	if (u_EmissionTransform)
	// 	{
	// 		emission = texture(u_Material.TextureEmission, IN.v_TexCoord + vec2(0.0, u_Time)).rgb; // translate over time
	// 		emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0; // fading 
	// 	}
	// }