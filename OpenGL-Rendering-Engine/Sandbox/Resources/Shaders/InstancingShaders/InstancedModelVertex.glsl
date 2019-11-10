/***************************************************************************
 * Filename		: InstancedModelVertex.glsl
 * Name			: Ori Lazar
 * Date			: 10/11/2019
 * Description	: Used to explore instancing capabilities of OpenGL and GLSL.
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
layout(location = 3) in vec2 a_Offsets; 

layout (std140) uniform Matrices2
{
	mat4 u_ViewProjection;
};

uniform mat4 u_Model;

out vec4 o_Color; 
out vec3 o_Position;
out vec3 o_Normal;
out vec2 o_TexCoord;

void main() 
{
	vec3 localPos =  vec3(a_Position.x + 100*a_Offsets.x, a_Position.y + 100*a_Offsets.y, a_Position.z);
	o_Color = vec4(a_Offsets.x/gl_InstanceID, a_Offsets.y/gl_InstanceID, 0,1);
	o_TexCoord = a_TexCoord; 
	o_Position = vec3(u_Model * vec4(localPos, 1.0));
	o_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	gl_Position = u_ViewProjection * u_Model * vec4(localPos,1);//vec4(a_Position.x + a_Offsets.x, a_Position.y + a_Offsets.y, a_Position.z, 1.0);

//	gl_Position = u_ViewProjection * a_Model * vec4(a_Position, 1.0);
// method 1 for small quantities of data
//	o_TexCoord = a_TexCoord; 
//	o_Position = vec3(u_Models[gl_InstanceID] * vec4(a_Position, 1.0));
//	o_Normal = mat3(transpose(inverse(u_Models[gl_InstanceID]))) * a_Normal;
//	gl_Position = u_ViewProjection * u_Models[gl_InstanceID] * vec4(a_Position, 1.0);
}