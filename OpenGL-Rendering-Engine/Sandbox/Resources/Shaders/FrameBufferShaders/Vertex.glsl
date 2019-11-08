/***************************************************************************
 * Filename		: Vertex.glsl
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: Can be used to pass a 2D quad to a fragment shader.
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
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 TexCoords;

void main()
{
    TexCoords = a_TexCoord;
    gl_Position = vec4(a_Position.x, a_Position.y ,0.0, 1.0); 
} 