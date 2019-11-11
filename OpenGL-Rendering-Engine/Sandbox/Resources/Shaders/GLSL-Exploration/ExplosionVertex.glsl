/***************************************************************************
 * Filename		: ExplosionVertex.glsl
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: Used to explore geometry shaders. Displays an exploding object. 
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
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout (std140) uniform Matrices
{
	mat4 u_ViewProjection;
};
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

out VS_OUT 
{
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
} vs_out;

void main()
{
	vs_out.TexCoord = a_TexCoord; 
	vs_out.Position = vec3(u_Model * vec4(a_Position, 1.0));
	vs_out.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}