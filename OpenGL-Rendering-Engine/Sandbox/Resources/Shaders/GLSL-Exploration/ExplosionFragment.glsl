/***************************************************************************
 * Filename		: NormalsFragment.glsl
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

layout(location = 0) out vec4 color;

uniform samplerCube u_Skybox;
uniform vec3 u_CameraPosition;

in GS_OUT 
{
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
} vs_in;


void main()
{
	color = vec4(vs_in.Normal,1.0);
}