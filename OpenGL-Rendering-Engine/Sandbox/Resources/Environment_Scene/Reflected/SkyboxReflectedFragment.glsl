/***************************************************************************
 * Filename		: ModelSkymapFragment.glsl
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Displays some very simple reflection / refraction skybox maps.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
//todo: Implement reflection maps
#version 330 core

layout (location = 0) out vec4 color;

layout (std140) uniform Camera_Uniforms 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
	vec3 CameraPosition;
	float _BoundaryPadding;
};

uniform samplerCube u_Skybox;

in vec3 o_Position;
in vec3 o_Normal;
//in vec2 o_TexCoord;

void main() 
{
	// if(u_Toggle == 0) // reflection (mirrors)
	// {
	vec3 I = normalize(o_Position - CameraPosition); // view direction
	vec3 R = reflect(I, normalize(o_Normal)); // reflection vector around object's normal vector
	color = vec4(texture(u_Skybox, R).rgb, 0.8);
	
	// }
	// else if(u_Toggle == 1) // refraction / snell's law (water-like surfaces)
	// {
	// 	float rir = 1 / 1.52; // make this configurable to different materials (from object index / to object index). Refractive index ratio...
	// 	vec3 I = normalize(o_Position - u_CameraPosition); // view vector, 
	// 	vec3 R = refract(I, normalize(o_Normal), rir); // refraction vector around object's normal 
	// 	color = vec4(texture(u_Skybox, R).rgb, 1.0);
	// }
}