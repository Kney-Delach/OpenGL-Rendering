/***************************************************************************
 * Filename		: Fragment.glsl
 * Name			: Ori Lazar
 * Date			: 08/11/2019
 * Description	: Implements post processing techniques on a custom frame buffered texture.
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
layout(location = 0) out vec4 color;
  
in vec2 TexCoords;

uniform sampler2D u_ScreenTexture;
uniform int  u_PostProcess;

const float offset = 1.0 / 300.0;
vec2 offsets[9] = vec2[] // This shader uses 3x3 kernels
(
	vec2(-offset,  offset), // top-left
	vec2( 0.0f,    offset), // top-center
	vec2( offset,  offset), // top-right
	vec2(-offset,  0.0f),   // center-left
	vec2( 0.0f,    0.0f),   // center-center
	vec2( offset,  0.0f),   // center-right
	vec2(-offset, -offset), // bottom-left
	vec2( 0.0f,   -offset), // bottom-center
	vec2( offset, -offset)  // bottom-right
);

void main()
{ 

	if(u_PostProcess == 0) // no post process effect
	{
		vec3 textureCol = texture(u_ScreenTexture, TexCoords).rgb;
		color = vec4(textureCol, 1.0);
	}
	else if(u_PostProcess == 1) // color inversion
	{
		color = vec4(vec3(1.0 - texture(u_ScreenTexture, TexCoords)), 1.0);
	}
	else if(u_PostProcess == 2) // weighted gray scale 
	{
		color = texture(u_ScreenTexture, TexCoords);
		float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
		color = vec4(average, average, average, 1.0);
	}
	else if(u_PostProcess == 3) // sharpen kernel
	{
		float kernel[9] = float[]
		(
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		);
    
		vec3 sampleTex[9];

		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(u_ScreenTexture, TexCoords.st + offsets[i]));
		}

		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		color = vec4(col, 1.0);
	}
	else if(u_PostProcess == 4) // blur kernel
	{
		float kernel[9] = float[]
		(
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16  
		);
    
		vec3 sampleTex[9];

		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(u_ScreenTexture, TexCoords.st + offsets[i]));
		}

		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		color = vec4(col, 1.0);
	}
	else if(u_PostProcess == 5) // edge detection
	{
		float kernel[9] = float[]
		(
			1, 1, 1,
			1, -8, 1,
			1, 1, 1
		);
    
		vec3 sampleTex[9];

		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(u_ScreenTexture, TexCoords.st + offsets[i]));
		}

		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];
		color = vec4(col, 1.0);
	}
}