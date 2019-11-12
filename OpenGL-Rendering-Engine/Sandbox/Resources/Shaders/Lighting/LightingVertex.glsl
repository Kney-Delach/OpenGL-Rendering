/***************************************************************************
 * Filename		: LightingVertex.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Vertex shader for lighting exploration.
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

layout (std140) uniform Camera_Matrices
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

out ShaderData 
{
	vec3 v_Position;
	vec3 v_Normal;
} OUT;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection; 

void main()
{
	OUT.v_Position = vec3(u_Model * vec4(a_Position,1));
	OUT.v_Normal = a_Normal;
	gl_Position = ViewProjectionMatrix * u_Model * vec4(a_Position,1.0);
}