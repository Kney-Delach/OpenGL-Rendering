/***************************************************************************
 * Filename		: ModelLoadVertex.glsl
 * Name			: Ori Lazar
 * Date			: 07/11/2019
 * Description	: This shader demonstrates drawing a model with and 
                  without a texture, using the same shader.
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
	o_Position = a_Position;
	o_Normal = a_Normal;
	o_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}