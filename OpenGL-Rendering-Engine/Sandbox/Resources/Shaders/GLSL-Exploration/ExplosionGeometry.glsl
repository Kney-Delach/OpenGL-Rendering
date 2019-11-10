/***************************************************************************
 * Filename		: ExplosionGeometry.glsl
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: Used to explore geometry shaders. Displays an exploding object. 
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

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT 
{
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
} gs_in[];

uniform float u_Time;

out GS_OUT 
{
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
} gs_out;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 4.0;
	vec3 direction = normal * ((sin(u_Time) + 1.0) / 2.0) * magnitude; 
	return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main()
{
	vec3 normal = GetNormal();
	gl_Position = explode(gl_in[0].gl_Position, normal);
	gs_out.TexCoord = gs_in[0].TexCoord;
	gs_out.Position = gs_in[0].Position;
	gs_out.Normal = gs_in[0].Normal;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	gs_out.TexCoord = gs_in[1].TexCoord;
	gs_out.Position = gs_in[1].Position;
	gs_out.Normal = gs_in[1].Normal;

	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	gs_out.TexCoord = gs_in[2].TexCoord;
	gs_out.Position = gs_in[2].Position;
	gs_out.Normal = gs_in[2].Normal;
	EmitVertex();
	EndPrimitive();
}