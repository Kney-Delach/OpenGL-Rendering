/***************************************************************************
 * Filename		: Fragment.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Fragment shader for the tesselation exploration.
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

layout(location = 0) out vec4 color;

in ShaderData 
{
	vec2 e_TexCoord;
	vec3 e_Position; 
} IN;

uniform sampler2D u_ColorMap;

void main()
{
	color =  vec4(IN.e_Position.y, IN.e_Position.y, IN.e_Position.y,1.0);// * texture(u_ColorMap, IN.e_TexCoord); // * vec4(IN.e_Position, 1.0); 
}