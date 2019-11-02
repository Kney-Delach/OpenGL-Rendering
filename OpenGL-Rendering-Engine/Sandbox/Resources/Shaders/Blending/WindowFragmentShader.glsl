/***************************************************************************
 * Filename		: WindowFragmentShader.glsl
 * Name			: Ori Lazar
 * Date			: 02/11/2019
 * Description	: This shader is used to demonstrate blending capabilities through OpenGL. 
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
	vec4 windowColour = texture(u_DiffuseTexture, o_TexCoord);
	
	gl_FragColor  = windowColour;
}