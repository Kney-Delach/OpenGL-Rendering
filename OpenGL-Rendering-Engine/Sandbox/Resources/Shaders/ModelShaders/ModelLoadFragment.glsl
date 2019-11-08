/***************************************************************************
 * Filename		: ModelLoadFragment.glsl
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: This shader demonstrates drawing a model with and 
                  without a texture, using the same shader.
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

uniform sampler2D u_DiffuseTexture; 

in vec3 o_Position;
in vec3 o_Normal;
in vec2 o_TexCoord;

void main() 
{
	vec4 temp = texture(u_DiffuseTexture, o_TexCoord);

	if(temp == vec4(1.0))
	{
		color = vec4(o_Normal,1.0);
	}
	else 
	{
		color = temp;
	}
}