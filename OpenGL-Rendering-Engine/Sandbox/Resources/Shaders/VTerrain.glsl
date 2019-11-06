#version 330

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_TextureMatrix;

out vec2 o_TexCoord; 

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	o_TexCoord = a_TexCoord;
}