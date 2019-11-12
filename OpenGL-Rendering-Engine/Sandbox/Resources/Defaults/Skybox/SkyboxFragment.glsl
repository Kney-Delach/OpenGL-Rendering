/***************************************************************************
 * Filename		: SkyboxFragment.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Draws a skybox fragment.
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

uniform samplerCube u_SkyboxTexture;

void main()
{
	color = texture(u_SkyboxTexture, o_TexCoord);
}