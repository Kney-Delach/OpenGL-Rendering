#version 330 

uniform sampler2D u_DiffuseTexture;

in vec2 o_TexCoord; 

out vec4 gl_FragColor;

void main()
{
	gl_FragColor = texture(u_DiffuseTexture, o_TexCoord); // o_Colour
}