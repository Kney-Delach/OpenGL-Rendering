/***************************************************************************
 * Filename		: InstancedModelFragment.glsl
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

layout (location = 0) out vec4 color;

uniform samplerCube u_Skybox;
uniform vec3 u_CameraPosition;
uniform int u_Toggle;

in vec3 o_Position;
in vec3 o_Normal;
in vec2 o_TexCoord;
in vec4 o_Color; 

void main() 
{
	vec3 I = normalize(o_Position - u_CameraPosition); // view direction
	vec3 R = reflect(I, normalize(o_Normal)); // reflection vector around object's normal vector
	color = vec4(texture(u_Skybox, R).rgb, 1.0);
}