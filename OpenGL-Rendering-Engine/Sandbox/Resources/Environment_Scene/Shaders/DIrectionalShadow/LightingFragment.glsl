/***************************************************************************
 * Filename		: LightingFragment.glsl
 * Name			: Ori Lazar
 * Date			: 15/11/2019
 * Description	: Fragment shader for Phong & Blinn-Phong Lighting and Shadows exploration.
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
	sampler2D TextureNormal;
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
#define NUMBER_OF_SPOT_LIGHTS 4 //todo: change this when adding more spot lights

layout (std140) uniform Light_Uniforms
{
	//PointLight PointLights[NUMBER_OF_POINT_LIGHTS];
	DirectionalLight DirectionalLights;
	//SpotLight SpotLights[NUMBER_OF_SPOT_LIGHTS];
};

layout(std140) uniform Camera_Uniforms
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
	vec4 v_LightSpaceFragCoord; // shadows
} IN;

//////////////////////////////////////////////////////////
// Shadow Mapping Uniforms ///////////////////////////////
//////////////////////////////////////////////////////////
uniform sampler2D u_ShadowMap;
//uniform vec3 u_LightPosition; 
//////////////////////////////////////////////////////////
// Other Uniforms ////////////////////////////////////////
//////////////////////////////////////////////////////////
uniform Material u_Material; 
// uniform bool u_BlinnPhong;
// uniform bool u_ActivateEmission;
// uniform bool u_EmissionTransform;
// uniform float u_Time; // glfwGetTime()

// ------------------ Main Shader Routines ------------------ //
//////////////////////////////////////////////////////////
// Diffuse Multiplier ////////////////////////////////////
//////////////////////////////////////////////////////////
 float Pi = 3.14159265;
 float CalculateDiffuseMultiplier(vec3 normal, vec3 lightDirection)
 {
 	return max(dot(normal, lightDirection), 0.0) / Pi; // divide by pi to conserve energy (thus staying consistent with specular calculation approach)
 }

// //////////////////////////////////////////////////////////
// // Specular Multiplier ///////////////////////////////////
// //////////////////////////////////////////////////////////
// // Energy Conservation article towards solving relative brightness problems: http://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/

 float CalculateSpecularMultiplier(vec3 lightDirection, vec3 normal, vec3 viewDirection)
 {
 	//if (u_BlinnPhong) // Blinn-Phong 
 	//{
 		float EnergyConservation = (8.0 + u_Material.Shininess) / (8.0 * Pi);
 		vec3 halfwayDirection = normalize(lightDirection + viewDirection);
 		return EnergyConservation * pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess);
 	//}
 	//else // Normal Phong 
 	//{
 	//	float EnergyConservation = (2.0 + u_Material.Shininess) / (2.0 * Pi);
 	//	vec3 reflectionDirection = reflect(-lightDirection, normal);
 	//	return EnergyConservation * pow(max(dot(viewDirection, reflectionDirection), 0.0), u_Material.Shininess);
 	//}
 }

// //////////////////////////////////////////////////////////
// // Attenuation Calculator ////////////////////////////////
// //////////////////////////////////////////////////////////
 vec3 CalculateAttenuationResults(vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 fragPosition, float constant, float lin, float quadratic)
 {
 	float distance = length(position - fragPosition);
 	float attenuation = 1.0 / (constant + lin * distance + quadratic * (distance * distance));
 	ambient *= attenuation;
 	diffuse *= attenuation;
 	specular *= attenuation;
 	return (ambient + diffuse + specular);
 }

// //////////////////////////////////////////////////////////
// // DIRECTIONAL LIGHT CALCULATION /////////////////////////
// //////////////////////////////////////////////////////////
 vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection, float shadow)
 {
 	vec3 lightDirection = normalize(light.Direction);

 	// diffuse
 	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

 	// specular 
 	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection);

 	// combine results 
 	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
 	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
 	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

 	return (ambient + (1 - shadow) * (diffuse + specular)); //(ambient + (1 - shadow)) * (diffuse + specular) * tempColor;
 }


// //////////////////////////////////////////////////////////
// // POINT LIGHT CALCULATION ///////////////////////////////
// //////////////////////////////////////////////////////////
// vec3 CalculatePointLights(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
// {
// 	vec3 lightDirection = normalize(light.Position - fragPosition);

// 	// diffuse shading
// 	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

// 	// specular shading
// 	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection); 

// 	// combine results
// 	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
// 	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
// 	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

// 	// attenuation
// 	return (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic));
// }
// //////////////////////////////////////////////////////////
// // SPOT LIGHT CALCULATION ////////////////////////////////
// //////////////////////////////////////////////////////////
// vec3 CalculateSpotLights(SpotLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
// {
// 	vec3 lightDirection = normalize(light.Position - fragPosition);

// 	// diffuse shading
// 	float diff = CalculateDiffuseMultiplier(normal, lightDirection);

// 	// specular shading
// 	float spec = CalculateSpecularMultiplier(lightDirection, normal, viewDirection); 

// 	// spotlight intensity
// 	float theta = dot(lightDirection, normalize(-light.Direction));
// 	float epsilon = light.CutoffInner - light.CutoffOuter;
// 	float intensity = clamp((theta - light.CutoffOuter) / epsilon, 0.0, 1.0);

// 	// combine results
// 	vec3 ambient = light.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
// 	vec3 diffuse = light.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));
// 	vec3 specular = light.Specular * spec * vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord));

// 	// Emission 
// 	vec3 emission = vec3(0);

// 	if (u_ActivateEmission && texture(u_Material.TextureSpecular, IN.v_TexCoord).r == 0)
// 	{
// 		emission = diff * texture(u_Material.TextureEmission, IN.v_TexCoord).rgb;
// 		if (u_EmissionTransform)
// 		{
// 			emission = texture(u_Material.TextureEmission, IN.v_TexCoord + vec2(0.0, u_Time)).rgb; // translate over time
// 			emission = emission * (sin(u_Time) * 0.5 + 0.5) * 2.0; // fading 
// 		}
// 	}
// 	// attenuation
// 	return intensity * (CalculateAttenuationResults(ambient, diffuse, specular, light.Position, fragPosition, light.AttenuationConstant, light.AttenuationLinear, light.AttenuationQuadratic) + emission);
// }

//////////////////////////////////////////////////////////
// Shadow Calculation ////////////////////////////////////
//////////////////////////////////////////////////////////
float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(u_ShadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	vec3 normal = normalize(IN.v_Normal);

	// check whether current frag pos is in shadow
	float bias = max(0.05 * (1.0 - dot(normal, DirectionalLights.Direction)), 0.005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

/////////////////////////////////////////////////////////////////////////
///////////////// Main //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void main()
{


	///////////////////////////////
	/// shadow main //////////////
	//////////////////////////////
	vec3 tempColor = texture(u_Material.TextureDiffuse, IN.v_TexCoord).rgb;

	// Performing all lighting calculations in world space
	vec3 normal = normalize(IN.v_Normal);


    //vec3 normal = normalize(IN.v_Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * tempColor;
    // diffuse
    vec3 lightDir = normalize(DirectionalLights.Direction - IN.v_FragPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(CameraPosition - IN.v_FragPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    
	// calculate shadow
    float shadow = ShadowCalculation(IN.v_LightSpaceFragCoord);  
	
	//color = vec4(shadow,0,0, 1);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * tempColor;

	color = vec4(lighting, 1);//vec4(lighting, 1.0);
}




	//// Point Light Calculations
	//for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
	//	result += CalculatePointLights(PointLights[i], normal, IN.v_FragPosition, viewDirection);

	//// Spot Light Calculations
	//for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)
	//	result += CalculateSpotLights(SpotLights[i], normal, IN.v_FragPosition, viewDirection);
		
	//color = vec4(result, 1.0);
