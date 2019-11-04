/***************************************************************************
 * Filename		: DEFAULT_VERTEX_SHADER_RED.glsl
 * Name			: Ori Lazar
 * Date			: 04/11/2019
 * Description	: This shader is displayed when no shader is provided to a 
                  gameobject WITH a model.
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

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

smooth out vec4 o_Color;

void main() 
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	o_Color = vec4(0.0f,0.0f,1.0f,1.0f);
}