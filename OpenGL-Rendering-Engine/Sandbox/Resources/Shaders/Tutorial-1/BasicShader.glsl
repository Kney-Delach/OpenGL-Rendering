#shader vertex
// Filename		: BasicShader.glsl
// Name			: Ori Lazar
// Date			: 29/10/2019
// Description	: This shader draws a single triangle to the screen and interpolates its colours.

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out Vertex 
{
	vec4 color;
} OUT;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform* vec4(a_Position, 1.0);
	OUT.color = a_Colour;
}

#shader fragment
#version 330 core 

in Vertex 
{
	vec4 color;
} IN;

out vec4 fragColour; 

void main()
{
	fragColour = IN.color;
}