/***************************************************************************
 * Filename		: NormalsFragment.glsl
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: Used to explore geometry shaders. Displays vertex normals in red.
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

void main()
{
	color = vec4(1.0,0.0,0.0,1.0);
}