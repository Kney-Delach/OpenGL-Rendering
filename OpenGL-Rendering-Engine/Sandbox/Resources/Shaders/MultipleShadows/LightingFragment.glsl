/***************************************************************************
 * Filename		: LightingFragment.glsl
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: Fragment shader for Phong & Blinn-Phong Lighting and and omnidirectional shadows 
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

// ------------------ Buffer Layouts ------------------ //

#define NUMBER_OF_POINT_LIGHTS 1 //todo: change this when adding more point lights

layout (std140) uniform Light_Uniforms
{
	PointLight PointLights[NUMBER_OF_POINT_LIGHTS];
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


in ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} IN;

// uniforms 
uniform samplerCube u_ShadowMap; // 
uniform Material u_Material; 

uniform float u_FarPlane;
uniform bool shadows;

// array of offset direction for sampling
//vec3 gridSamplingDisk[20] = vec3[]
//(
//	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
//	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
//	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
//	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
//	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
//);

// ------------------ Main Shader Routines ------------------ //

//////////////////////////////////////////////////////////
// Diffuse Multiplier ////////////////////////////////////
//////////////////////////////////////////////////////////
 float Pi = 3.14159265;
 float CalculateDiffuseMultiplier(vec3 normal, vec3 lightDirection)
 {
 	return max(dot(normal, lightDirection), 0.0) / Pi; // divide by pi to conserve energy (thus staying consistent with specular calculation approach)
 }

////////////////////////////////////////////////////////////
//// Specular Multiplier ///////////////////////////////////
////////////////////////////////////////////////////////////
//// Energy Conservation article towards solving relative brightness problems: http://www.rorydriscoll.com/2009/01/25/energy-conservation-in-games/
 float CalculateSpecularMultiplier(vec3 lightDirection, vec3 normal, vec3 viewDirection)
 {
 	float EnergyConservation = (8.0 + u_Material.Shininess) / (8.0 * Pi);
 	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
 	return EnergyConservation * pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess);
 }

//W//////////////////////////////////////////////////////////
//W// Attenuation Calculator ////////////////////////////////
//W//////////////////////////////////////////////////////////
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
// // POINT LIGHT CALCULATION ///////////////////////////////
// //////////////////////////////////////////////////////////
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


 // is the fragment to light vector to sample from the depth map    
 //float closestDepth = texture(u_ShadowMap, fragToLight).r;

 //it is currently in linear range between [0,1], let's re-transform it back to original depth value	
 //closestDepth *= far_plane;


//////////////////////////////////////////////////////////
// Shadow Calculation ////////////////////////////////////
//////////////////////////////////////////////////////////
float ShadowCalculation(vec3 fragPosition)
{
	vec3 fragToLightDirection = fragPosition - PointLights[0].Position; // vector between fragment position and light position

	float currentDepth = length(fragToLightDirection); // translate linear depth as the length between the fragment and light position

	float shadow = 0.0;
	float bias = 0.05;
	float samples = 4.0;
	float offset = 0.1;
	for (float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for (float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for (float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(u_ShadowMap, fragToLightDirection + vec3(x, y, z)).r;
				closestDepth *= u_FarPlane;
				if (currentDepth - bias > closestDepth)
					shadow += 1.0;
			}
		}
	}
	shadow /= (samples * samples * samples);

	return shadow;

	////////////////////////////////////////////////////

	// test for shadows
	//float bias = 0.05; // we use a much larger bias since depth is now in [near_plane, far_plane] range
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	////////////////////////////////////////////////////

	//float shadow = 0.0;
	//float bias = 0.05;// 15;
	//int samples = 20;
	//float viewDistance = length(CameraPosition - fragPosition);
	//float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0; //25.0;
	//for (int i = 0; i < samples; ++i)
	//{
	//	float closestDepth = texture(u_ShadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
	//	closestDepth *= far_plane;   // undo mapping [0;1]
	//	if (currentDepth - bias > closestDepth)
	//		shadow += 1.0;
	//}
	//shadow /= float(samples);

	//////////////// Debug Display Section ////////////////// 

	// display closestDepth as debug (to visualize depth cubemap)

	//color = vec4(vec3(closestDepth / far_plane), 1.0);

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
	vec3 normal = normalize(IN.v_Normal);
	vec3 lightColor = vec3(0.3);
	// ambient
	vec3 ambient = 0.3 * tempColor;
	// diffuse
	vec3 lightDir = normalize(PointLights[0].Position - IN.v_FragPosition);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;
	// specular
	vec3 viewDir = normalize(CameraPosition - IN.v_FragPosition); // verify camera position is view position
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	vec3 specular = spec * lightColor;

	// calculate shadow
	float shadow = shadows ? ShadowCalculation(IN.v_FragPosition) : 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * tempColor;

	color = vec4(lighting, 1.0);
}