#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

smooth out vec4 o_Color;

void main() 
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	o_Color = a_Colour;
}