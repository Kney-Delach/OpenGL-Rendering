/***************************************************************************
 * Filename		: ModelFragment2.glsl
 * Name			: Ori Lazar
 * Date			: 09/11/2019
 * Description	: Used to explore the fragment shader, can be seen in the 
                  GLSL-Exploration scene.
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

void main() 
{
	vec3 I = normalize(o_Position - u_CameraPosition); // view direction
	vec3 R = reflect(I, normalize(o_Normal)); // reflection vector around object's normal vector
	color = vec4(1.0,0.0,1.0,1.0) * vec4(texture(u_Skybox, R).rgb, 1.0);
}