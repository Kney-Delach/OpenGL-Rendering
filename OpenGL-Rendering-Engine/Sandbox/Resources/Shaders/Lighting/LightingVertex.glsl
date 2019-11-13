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

//todo: An optimization would be to calculate this value on the cpu and then pass that through to the gpu, as it is a costly operation.
//todo: Calculate this on the cpu: mat3(transpose(inverse(u_Model))) -> Normal Matrix

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
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} OUT;

uniform mat4 u_Model;

void main()
{
	OUT.v_FragPosition = vec3(RealViewMatrix * u_Model * vec4(a_Position,1)); // view space vertex position
	OUT.v_Normal = mat3(transpose(inverse(RealViewMatrix * u_Model))) * a_Normal; // generate the normal matrix  (necessary when model is scales / translated)
	OUT.v_TexCoord = a_TexCoord;
	gl_Position = ViewProjectionMatrix * u_Model * vec4(a_Position, 1.0);
}