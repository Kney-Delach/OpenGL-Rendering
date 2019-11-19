/***************************************************************************
 * Filename		: LightingVertex.glsl
 * Name			: Ori Lazar
 * Date			: 12/11/2019
 * Description	: Vertex shader for lighting exploration.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/

//todo: An optimization would be to calculate this value on the cpu and then pass that through to the gpu, as it is a costly operation.
//todo: Calculate this on the cpu: mat3(transpose(inverse(u_Model))) -> Normal Matrix

#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout (std140) uniform Camera_Matrices 
{
	mat4 RealViewMatrix;
	mat4 SkyboxViewMatrix;
	mat4 ProjectionMatrix;
	mat4 ViewProjectionMatrix;
	vec3 CameraPosition;
	float _BoundaryPadding;
};

out ShaderData 
{
	vec3 v_FragPosition;
	//vec3 v_Normal;
	vec2 v_TexCoord;
	mat3 v_TBN;
} OUT;

uniform mat4 u_Model;

void main()
{
	vec3 T = normalize(vec3(u_Model * vec4(a_Tangent, 0.0)));
	vec3 B = normalize(vec3(u_Model * vec4(cross(a_Tangent, a_Normal), 0.0)));
	vec3 N = normalize(vec3(u_Model * vec4(a_Normal, 0.0)));
	OUT.v_TBN = transpose(mat3(T, B, N));

	OUT.v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
	//OUT.v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal; // generate the normal matrix  (necessary when model is scales / translated)
	OUT.v_TexCoord = a_TexCoord;
	gl_Position = ViewProjectionMatrix * u_Model * vec4(a_Position, 1.0);
}