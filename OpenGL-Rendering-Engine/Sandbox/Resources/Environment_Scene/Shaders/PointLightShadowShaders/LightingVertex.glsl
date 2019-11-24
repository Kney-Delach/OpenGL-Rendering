/***************************************************************************
 * Filename		: LightingVertex.glsl
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: Vertex shader for lighting and omnidirectional shadows 
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

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

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
	vec3 v_Normal;
	vec2 v_TexCoord;
} OUT;

uniform mat4 u_Model;
//uniform bool reverse_normals; 

void main()
{
	OUT.v_FragPosition = vec3(u_Model * vec4(a_Position, 1.0));
	OUT.v_TexCoord = a_TexCoord;

	// when drawing a cube from the inside, reversing the normals results in successful lighting calculations
	//if(reverse_normals)
	//	OUT.v_Normal = transpose(inverse(mat3(u_Model))) * (-1.0 * a_Normal); 
	//else 
	OUT.v_Normal = transpose(inverse(mat3(u_Model))) * a_Normal;

	gl_Position = ViewProjectionMatrix * u_Model * vec4(a_Position, 1.0);
}