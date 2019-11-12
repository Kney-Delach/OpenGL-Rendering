/***************************************************************************
 * Filename		: Vertex.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Vertex shader for the tesselation exploration.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out ShaderData 
{
	vec2 v_TexCoord;
	vec3 v_Position;
} OUT;

void main()
{
	OUT.v_TexCoord = a_TexCoord;
	OUT.v_Position = a_Position;
	gl_Position = vec4(a_Position, 1.0);
}