/***************************************************************************
 * Filename		: PointLightReflectedFragment.glsl
 * Name			: Ori Lazar
 * Date			: 26/11/2019
 * Description	: Displays some very simple reflection 
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
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
uniform vec3 u_Color; 

in vec3 o_Position;
in vec3 o_Normal;

void main() 
{
	float rir = 1 / 1.52; // make this configurable to different materials (from object index / to object index). Refractive index ratio...
	vec3 I = normalize(o_Position - CameraPosition); // view vector, 
	vec3 R = refract(I, normalize(o_Normal), rir); // refraction vector around object's normal 
	vec3 col = texture(u_Skybox, R).rgb * u_Color;
	color = vec4(col, 0.8);
}