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
#version 420 

layout (quads, cw) in; // Tesselation using quads and clockwise winding

layout (std140) uniform Camera_Matrices
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
};

in ShaderData 
{
	vec3 c_Position;
	vec2 c_ColorTexCoord;
	vec2 c_HeightmapTexCoord;
} IN[];

out ShaderData 
{
	vec2 e_TexCoord;
	float e_TerrainScale;
	vec3 e_Position; 
	float e_PositionY;
} OUT;

uniform float u_TerrainScale;
uniform sampler2D u_HeightMap;


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
	
	vec2 texCoordHeightmap = QuadMixVec2(IN[0].c_HeightmapTexCoord, IN[1].c_HeightmapTexCoord, IN[2].c_HeightmapTexCoord, IN[3].c_HeightmapTexCoord);
	vec2 texCoordColormap = QuadMixVec2(IN[0].c_ColorTexCoord, IN[1].c_ColorTexCoord, IN[2].c_ColorTexCoord, IN[3].c_ColorTexCoord);

	vec3 position = QuadMixVec3(IN[0].c_Position, IN[1].c_Position * u_TerrainScale, IN[2].c_Position * u_TerrainScale, IN[3].c_Position * u_TerrainScale);

	position.y = 200 * texture(u_HeightMap, texCoordHeightmap).r;
	 
	gl_Position = ViewProjectionMatrix * vec4(position, 1.0);

	OUT.e_PositionY = position.y;
	OUT.e_Position = position;
	OUT.e_TexCoord = texCoordColormap;
	OUT.e_TerrainScale = u_TerrainScale;
}