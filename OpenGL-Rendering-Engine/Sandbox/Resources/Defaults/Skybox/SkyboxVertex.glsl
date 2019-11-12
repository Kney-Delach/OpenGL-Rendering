/***************************************************************************
 * Filename		: SkyboxVertex.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Processes a skybox vertex.
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

layout(location = 0) in vec3 a_Position;

layout (std140) uniform Camera_Matrices
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

out vec3 o_TexCoord;

void main()
{
	o_TexCoord = a_Position;
	vec4 pos = ProjectionMatrix * SkyboxViewMatrix * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
} 