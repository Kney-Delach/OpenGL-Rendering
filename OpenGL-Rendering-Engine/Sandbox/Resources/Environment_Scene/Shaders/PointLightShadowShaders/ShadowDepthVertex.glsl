/***************************************************************************
 * Filename		: ShadowDepthVertex.glsl
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: Vertex shader for lighting and omnidirectional shadows
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

uniform mat4 u_Model;

void main()
{
	gl_Position = u_Model * vec4(a_Position, 1.0);
}