/***************************************************************************
 * Filename		: InstancedModelVertex.glsl
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: Used to explore instancing capabilities of OpenGL and GLSL.
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

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_Models; 

layout (std140) uniform Matrices2
{
	mat4 u_ViewProjection;
};

out vec4 o_Color; 
out vec3 o_Position;
out vec3 o_Normal;
out vec2 o_TexCoord;

void main() 
{
	o_TexCoord = a_TexCoord; 
	o_Position = vec3(a_Models * vec4(a_Position, 1.0));
	o_Normal = mat3(transpose(inverse(a_Models))) * a_Normal;
	gl_Position = u_ViewProjection * a_Models * vec4(a_Position, 1.0);
}