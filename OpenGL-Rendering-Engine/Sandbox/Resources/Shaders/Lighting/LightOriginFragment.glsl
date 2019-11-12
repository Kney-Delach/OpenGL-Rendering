/***************************************************************************
 * Filename		: LightOriginFragment.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Fragment shader for lighting exploration.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#version 330 

layout(location = 0) out vec4 color;

in ShaderData 
{
	vec3 v_Color;
} IN;

void main()
{
	color = vec4(IN.v_Color,1.0);
}