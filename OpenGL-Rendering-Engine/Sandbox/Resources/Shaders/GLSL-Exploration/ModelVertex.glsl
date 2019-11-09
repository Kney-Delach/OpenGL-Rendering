/***************************************************************************
 * Filename		: ModelVertex.glsl
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Processes model vertices in the GLSL exploration scene.
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

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec3 o_Position;
out vec3 o_Normal;
out vec2 o_TexCoord;

void main() 
{
	o_TexCoord = a_TexCoord; 
	o_Position = vec3(u_Model * vec4(a_Position, 1.0));
	o_Normal = mat3(transpose(inverse(u_Model))) * a_Normal; // normal calaculation 
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}