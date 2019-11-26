#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 1024) out;

in ShaderData
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} IN[];

out ShaderData
{
	vec3 v_FragPosition;
	vec3 v_Normal;
	vec2 v_TexCoord;
} OUT;

uniform float u_Time;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 6.0;
	vec3 direction = normal * (((u_Time - 118.5f) + 1.0) / 2.0) * magnitude;
	if (u_Time > 118.5f)
		return position + vec4(direction, 0.0);
	else
		return position;
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

	gl_Position = explode(gl_in[0]	.gl_Position, normal);
	OUT.v_TexCoord = IN[0].v_TexCoord;
	OUT.v_FragPosition = IN[0].v_FragPosition;
	OUT.v_Normal = IN[0].v_Normal;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal);
	OUT.v_TexCoord = IN[1].v_TexCoord;
	OUT.v_FragPosition = IN[1].v_FragPosition;
	OUT.v_Normal = IN[1].v_Normal;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal);
	OUT.v_TexCoord = IN[2].v_TexCoord;
	OUT.v_FragPosition = IN[2].v_FragPosition;
	OUT.v_Normal = IN[2].v_Normal;
	EmitVertex();

	EndPrimitive();
}