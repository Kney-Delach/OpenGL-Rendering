/***************************************************************************
 * Filename		: ShadowDepthFragment.glsl
 * Name			: Ori Lazar
 * Date			: 18/11/2019
 * Description	: Fragment shader for lighting and omnidirectional shadows
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

in vec4 FragPos;

uniform vec3 lightPos;
uniform float u_FarPlane;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPos);

	// map to [0;1] range by dividing by far_plane
	lightDistance = lightDistance / u_FarPlane;

	// write this as modified depth
	gl_FragDepth = lightDistance;
}