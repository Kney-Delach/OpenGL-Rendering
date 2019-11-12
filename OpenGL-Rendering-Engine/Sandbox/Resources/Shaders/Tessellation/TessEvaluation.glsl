/***************************************************************************
 * Filename		: TessEvaluation.glsl
 * Name			: Ori Lazar
 * Date			: 11/11/2019
 * Description	: Tesselation Evaluation shader used in tesselation exploration.
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

layout (quads, cw) in; // Tesselation using quads and clockwise winding

layout (std140) uniform Camera_Matrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

uniform float u_TerrainScale;
uniform sampler2D u_HeightMap;

in ShaderData 
{
	vec2 c_TexCoord;
	vec3 c_Position;
} IN[];

out ShaderData 
{
	vec2 e_TexCoord;
	vec3 e_Position; 
} OUT;

vec4 QuadMixVec4(vec4 v1, vec4 v2, vec4 v3, vec4 v4) 
{
	vec4 position1 = mix(v1, v2, gl_TessCoord.x);			// Interpolate along the bottom edge using the x component of the tesselation co-ordinate
	vec4 position2 = mix(v3, v4, gl_TessCoord.x);			// Interpolate along the top edge using the x component of the tesselation co-ordinate
	return mix(position1, position2, gl_TessCoord.y);		// Interpolate between position 1 and position 2 using the y component of the tesselation co-ordinate
}

vec3 QuadMixVec3(vec3 v1, vec3 v2, vec3 v3, vec3 v4) 
{
	vec3 position1 = mix(v1, v2, gl_TessCoord.x);
	vec3 position2 = mix(v3, v4, gl_TessCoord.x);
	return mix(position1, position2, gl_TessCoord.y);
}

vec2 QuadMixVec2(vec2 v1, vec2 v2, vec2 v3, vec2 v4) 
{
	vec2 position1 = mix(v1, v2, gl_TessCoord.x);
	vec2 position2 = mix(v3, v4, gl_TessCoord.x);
	return mix(position1, position2, gl_TessCoord.y);
}

void main()
{
	//gl_Position = QuadMixVec4(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position,gl_in[3].gl_Position);
	
	vec2 texCoord = QuadMixVec2(IN[0].c_TexCoord, IN[1].c_TexCoord, IN[2].c_TexCoord, IN[3].c_TexCoord);

	vec3 position = QuadMixVec3(IN[0].c_Position, IN[1].c_Position, IN[2].c_Position, IN[3].c_Position);

	position.y += u_TerrainScale * texture(u_HeightMap, texCoord).r;
	 
	 gl_Position = ViewProjectionMatrix * vec4(position, 1.0);

	OUT.e_TexCoord = texCoord;
	OUT.e_Position = position;
}