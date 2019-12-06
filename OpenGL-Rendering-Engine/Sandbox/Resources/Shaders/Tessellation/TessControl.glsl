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
		vec4 p0 = ViewProjectionMatrix * gl_in[0].gl_Position;
		vec4 p1 = ViewProjectionMatrix * gl_in[1].gl_Position;
		vec4 p2 = ViewProjectionMatrix * gl_in[2].gl_Position;
		vec4 p3 = ViewProjectionMatrix * gl_in[3].gl_Position;

		p0 /= p0.w;
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;

		if (p0.z <= 0.0 || p1.z <= 0.0 || p2.z <= 0.0 || p3.z <= 0.0)
		{
			// don't tesselate vertecis behind camera frustum 
			gl_TessLevelOuter[0] = 0.;
			gl_TessLevelOuter[1] = 0.;
			gl_TessLevelOuter[2] = 0.;
			gl_TessLevelOuter[3] = 0.;
		}
		else
		{
			float l0 = length(p2.xy - p0.xy) * 16.0 + 1.0;
			float l1 = length(p3.xy - p2.xy) * 16.0 + 1.0;
			float l2 = length(p3.xy - p1.xy) * 16.0 + 1.0;
			float l3 = length(p1.xy - p0.xy) * 16.0 + 1.0;

			gl_TessLevelOuter[0] = l0;
			gl_TessLevelOuter[1] = l1;
			gl_TessLevelOuter[2] = l2;
			gl_TessLevelOuter[3] = l3;

			// Inner tesselation factors
			gl_TessLevelInner[0] = min(l1, l3);
			gl_TessLevelInner[1] = min(l0, l2);

		}

		//// Inner tesselation factors
		//gl_TessLevelInner[0] = 100.0;
		//gl_TessLevelInner[1] = 100.0;

	//	// Outer tesselation factors
	//	gl_TessLevelOuter[0] = 1000.0;
	//	gl_TessLevelOuter[1] = 1000.0;
	//	gl_TessLevelOuter[2] = 1000.0;
	//	gl_TessLevelOuter[3] = 1000.0;
	//}
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	OUT[gl_InvocationID].c_Position = IN[gl_InvocationID].v_Position;
	OUT[gl_InvocationID].c_ColorTexCoord = IN[gl_InvocationID].v_ColorTexCoord;
	OUT[gl_InvocationID].c_HeightmapTexCoord = IN[gl_InvocationID].v_HeightmapTexCoord;
}