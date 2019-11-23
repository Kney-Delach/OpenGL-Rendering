#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

#define NUMBER_OF_SPOT_LIGHTS 4

layout(std140) uniform Light_Space_Uniforms
{
	mat4 LightSpaceMatrix[NUMBER_OF_SPOT_LIGHTS]; //todo: change the number of spot lights dynamically? 
};

layout(std140) uniform Directional_Light_Space_Uniforms
{
	mat4 DirectionalLightSpaceMatrix; 
};

uniform int u_ShadowIndex; 
uniform mat4 u_Model;
uniform bool IsDirectional; 

void main()
{
	if(!IsDirectional)
		gl_Position = LightSpaceMatrix[u_ShadowIndex] * u_Model * vec4(a_Position, 1.0);
	else 
		gl_Position = DirectionalLightSpaceMatrix * u_Model * vec4(a_Position, 1.0);

}