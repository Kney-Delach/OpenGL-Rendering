/***************************************************************************
 * Filename		: LightingFragment.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Fragment shader for lighting exploration.
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

// ------------------ Data Structures for Shader Uniforms ------------------ //

struct Material 
{
//	vec3 Ambient;	// use this when no diffuse texture is included
//	vec3 Diffuse;	// use this when no diffuse texture is included
//	vec3 Specular;	// use this when no specular texture is included 
	float Shininess;
	sampler2D TextureDiffuse;
	sampler2D TextureSpecular;
};

//todo: Find out why not padding this doesn't break it....
struct Light
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	vec3 Position;
};

// ------------------ Buffer Layouts ------------------ //

layout (std140) uniform Light_Uniforms
{
	Light LightA;
};

layout (std140) uniform Camera_Matrices 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

// ------------------ Input from previous shader stages ------------------ //

in ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} IN;

uniform Material u_Material; 

// ------------------ Main Shader Routines ------------------ //

vec3 CalculateAmbient()
{
	vec3 ambient = LightA.Ambient * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord));   ;//u_Material.Ambient;
	return ambient;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDirection)
{

	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = LightA.Diffuse * diff * vec3(texture(u_Material.TextureDiffuse, IN.v_TexCoord)); // u_Material.Diffuse;
	return diffuse;
}

vec3 CalculateSpecular(vec3 normal, vec3 lightDirection)
{
	vec3 camerViewDirection = normalize(-IN.v_FragPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);  
	float spec = pow(max(dot(camerViewDirection, reflectionDirection), 0.0), u_Material.Shininess);
	vec3 specular = LightA.Specular * spec *  vec3(texture(u_Material.TextureSpecular, IN.v_TexCoord)); //u_Material.Specular; 
	return specular;
}

void main()
{
	// ambient
	vec3 ambient = CalculateAmbient();
	vec3 normal = normalize(IN.v_Normal);
	vec3 lightDirection = normalize(vec3(RealViewMatrix * vec4(LightA.Position, 1.0)) - IN.v_FragPosition);

	// diffuse 
	vec3 diffuse = CalculateDiffuse(normal, lightDirection);

	// specular 
	vec3 specular = CalculateSpecular(normal, lightDirection);

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
}