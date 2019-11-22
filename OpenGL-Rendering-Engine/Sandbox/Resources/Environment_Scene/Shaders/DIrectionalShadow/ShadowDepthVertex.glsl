#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

#define NUMBER_OF_SPOT_LIGHTS 2

layout(std140) uniform Light_Space_Uniforms
{
	mat4 LightSpaceMatrix[NUMBER_OF_SPOT_LIGHTS]; //todo: change the number of spot lights dynamically? 
};

uniform int u_ShadowIndex; 
uniform mat4 u_Model;

void main()
{
	gl_Position = LightSpaceMatrix[u_ShadowIndex] * u_Model * vec4(a_Position, 1.0);
}