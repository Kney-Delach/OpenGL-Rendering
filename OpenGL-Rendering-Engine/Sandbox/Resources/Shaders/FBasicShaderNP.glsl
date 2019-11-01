#version 330 core 

noperspective in vec4 o_Color;

out vec4 fragColour; 

void main()
{
	fragColour = o_Color;
}