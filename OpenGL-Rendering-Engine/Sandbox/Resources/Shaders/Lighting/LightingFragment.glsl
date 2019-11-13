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

layout(location = 0) out vec4 color;

layout (std140) uniform Light_Uniforms
{
	vec3 CameraPosition;  // This was necessary when doing lighting calculations in world space
	float padding;
};

layout (std140) uniform Camera_Matrices 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

in ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
} IN;

uniform vec3 u_ObjectColor;
uniform vec3 u_LightSourceColor;
uniform vec3 u_LightSourcePosition;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * u_LightSourceColor;

	// diffuse 
	vec3 normal = normalize(IN.v_Normal);
	vec3 lightDirection = normalize(vec3(RealViewMatrix * vec4(u_LightSourcePosition, 1.0)) - IN.v_FragPosition); // order relevant when calculating reflectionDirection
	float diff = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = diff * u_LightSourceColor;

	// specular 
	float specularStrength = 0.5;
	//vec3 cameraViewDirection = normalize(CameraPosition - IN.v_FragPosition); // When doing lighting calculations in world space 
	vec3 cameraViewDirection = normalize(-IN.v_FragPosition); // View space calulation, as view direction is (0,0,0), -> -Pos
	vec3 reflectionDirection = reflect(-lightDirection, normal); // reflect expects first vector to point FROM light source towards fragment position
	
	float shininess = 256; // The higher, the more reflective it is.
	float spec = pow(max(dot(cameraViewDirection, reflectionDirection), 0.0), shininess); 
	vec3 specular = specularStrength * spec * u_LightSourceColor; 

	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	color = vec4(result, 1.0);

}