/***************************************************************************
 * Filename		: DEFAULT_FRAGMENT_SHADER.glsl
 * Name			: Ori Lazar
 * Date			: 04/11/2019
 * Description	: This shader is displayed when no shader is provided to a 
                  gameobject WITH a model.
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

smooth in vec4 o_Color;

out vec4 fragColour; 

void main()
{
	fragColour = o_Color;
}