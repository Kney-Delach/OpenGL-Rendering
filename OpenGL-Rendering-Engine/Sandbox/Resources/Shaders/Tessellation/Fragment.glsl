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
#version 420 

layout(location = 0) out vec4 color;

in ShaderData 
{
	vec2 e_TexCoord;
	float e_TerrainScale;
	float e_Position;
	float e_PositionY;
} IN;

uniform sampler2D u_ColorMap;

void main()
{
	color = vec4(IN.e_PositionY/IN.e_TerrainScale, IN.e_PositionY/IN.e_TerrainScale, IN.e_PositionY/IN.e_TerrainScale, 1.0)* texture(u_ColorMap, IN.e_TexCoord);//vec4(IN.e_PositionY/IN.e_TerrainScale,IN.e_PositionY/IN.e_TerrainScale,IN.e_PositionY/IN.e_TerrainScale,1.0);
}