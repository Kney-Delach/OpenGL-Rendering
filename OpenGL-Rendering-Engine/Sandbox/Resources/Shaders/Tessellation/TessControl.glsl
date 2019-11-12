/***************************************************************************
 * Filename		: TessControl.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Tesselation Control shader used in tesselation exploration.
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

layout (vertices = 4) out; // Tesselation using quads 

in ShaderData 
{
	vec2 v_TexCoord;
	vec3 v_Position;
} IN[];

out ShaderData 
{
	vec2 c_TexCoord;
	vec3 c_Position;
} OUT[];

void main()
{
	if (gl_InvocationID == 0)
	{
		// Inner tesselation factors
		gl_TessLevelInner[0] = 9.0;
		gl_TessLevelInner[1] = 7.0;
		
		// Outer tesselation factors
		gl_TessLevelOuter[0] = 3.0;
		gl_TessLevelOuter[1] = 5.0;
		gl_TessLevelOuter[2] = 3.0;
		gl_TessLevelOuter[3] = 5.0;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	OUT[gl_InvocationID].c_TexCoord = IN[gl_InvocationID].v_TexCoord;
	OUT[gl_InvocationID].c_Position = IN[gl_InvocationID].v_Position;
}