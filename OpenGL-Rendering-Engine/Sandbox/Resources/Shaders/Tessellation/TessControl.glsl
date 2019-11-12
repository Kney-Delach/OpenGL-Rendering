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
#version 420 

layout (vertices = 4) out; // Tesselation using quads 

layout (std140) uniform Camera_Matrices
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

in ShaderData 
{
	vec3 v_Position;
	vec2 v_ColorTexCoord;
	vec2 v_HeightmapTexCoord;
} IN[];

out ShaderData 
{
	vec3 c_Position;
	vec2 c_ColorTexCoord;
	vec2 c_HeightmapTexCoord;
} OUT[];

void main()
{
	if (gl_InvocationID == 0)
	{
		// Inner tesselation factors
		gl_TessLevelInner[0] = 100.0;
		gl_TessLevelInner[1] = 100.0;
		
		// Outer tesselation factors
		gl_TessLevelOuter[0] = 1000.0;
		gl_TessLevelOuter[1] = 1000.0;
		gl_TessLevelOuter[2] = 1000.0;
		gl_TessLevelOuter[3] = 1000.0;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	OUT[gl_InvocationID].c_Position = IN[gl_InvocationID].v_Position;
	OUT[gl_InvocationID].c_ColorTexCoord = IN[gl_InvocationID].v_ColorTexCoord;
	OUT[gl_InvocationID].c_HeightmapTexCoord = IN[gl_InvocationID].v_HeightmapTexCoord;
}