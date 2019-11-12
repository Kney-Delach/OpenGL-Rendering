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
#version 420

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_ColorTexCoord;
layout(location = 2) in vec2 a_HeightmapTexCoord;


out ShaderData 
{
	vec3 v_Position;
	vec2 v_ColorTexCoord;
	vec2 v_HeightmapTexCoord;
} OUT;

void main()
{
	gl_Position = vec4(a_Position,1.0);
	OUT.v_Position = a_Position;
	OUT.v_ColorTexCoord = a_ColorTexCoord;
	OUT.v_HeightmapTexCoord = a_HeightmapTexCoord;
}