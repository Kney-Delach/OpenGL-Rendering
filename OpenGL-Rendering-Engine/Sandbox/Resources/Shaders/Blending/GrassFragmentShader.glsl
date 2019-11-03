/***************************************************************************
 * Filename		: GrassFragmentShader.glsl
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This shader discards alpha values of grass textures. 
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

uniform sampler2D u_DiffuseTexture;

in vec2 o_TexCoord; 
in vec4 o_Colour; 

out vec4 gl_FragColor;

void main()
{
	vec4 grassColour = texture(u_DiffuseTexture, o_TexCoord);
	if(grassColour.a < 0.1)
		discard; 
	gl_FragColor  = grassColour;
}