/***************************************************************************
 * Filename		: Fragment.glsl
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Implements a skybox 
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

in vec3 o_TexCoord;

uniform samplerCube u_Skybox;

void main()
{
	color = texture(u_Skybox, o_TexCoord);
}