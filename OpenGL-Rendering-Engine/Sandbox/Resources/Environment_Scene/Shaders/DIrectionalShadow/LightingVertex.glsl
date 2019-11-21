/***************************************************************************
 * Filename		: LightingVertex.glsl
 * Name			: Ori Lazar
 * Date			: 15/11/2019
 * Description	: Vertex shader for lighting and Shadows exploration.
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

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout(std140) uniform Camera_Uniforms
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
	vec3 CameraPosition;
	float _BoundaryPadding;
};

out ShaderData 
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
	vec4 v_LightSpaceFragCoord; // shadows
} OUT;

uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix; // shadows

void main()
{
	OUT.v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
	OUT.v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	OUT.v_TexCoord = a_TexCoord;
	OUT.v_LightSpaceFragCoord = u_LightSpaceMatrix * vec4(OUT.v_FragPosition, 1.0);//OUT.v_FragPosition, 1.0); //shadows
	gl_Position = ViewProjectionMatrix * u_Model * vec4(a_Position, 1.0);
}