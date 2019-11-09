/***************************************************************************
 * Filename		: ModelFragment.glsl
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
	if(gl_FragCoord.x < 640)//if(u_Toggle == 0) // reflection (mirrors)
	{
		vec3 I = normalize(o_Position - u_CameraPosition); // view direction
		vec3 R = reflect(I, normalize(o_Normal)); // reflection vector around object's normal vector
		color = vec4(texture(u_Skybox, R).rgb, 1.0);
	}
	else if(gl_FragCoord.x >= 640)//if(u_Toggle == 1) // refraction / snell's law (water-like surfaces)
	{
		float rir = 1 / 1.52; // make this configurable to different materials (from object index / to object index). Refractive index ratio...
		vec3 I = normalize(o_Position - u_CameraPosition); // view vector, 
		vec3 R = refract(I, normalize(o_Normal), rir); // refraction vector around object's normal 
		color = vec4(texture(u_Skybox, R).rgb, 1.0);
	}
}