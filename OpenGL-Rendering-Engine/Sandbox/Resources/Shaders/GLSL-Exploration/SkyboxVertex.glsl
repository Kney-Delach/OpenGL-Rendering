/***************************************************************************
 * Filename		: Vertex.glsl
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Implements a skybox.
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

out vec3 o_TexCoord;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
	o_TexCoord = a_Position;
	vec4 pos = u_ProjectionMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
	gl_Position = pos.xyww;
} 