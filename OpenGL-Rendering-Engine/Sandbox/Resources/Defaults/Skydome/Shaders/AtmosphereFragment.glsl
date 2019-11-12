#version 430

layout(location = 0) out vec4 outputColor;

in vec3 worldPosition;

const vec3 baseColor = vec3(0.18,0.27,0.47);

void main()
{
	//float red = //normalize(worldPosition.y);
	//float green = //normalize(worldPosition.y);
	float blue = normalize(worldPosition.y);
	
	outputColor = vec4(0,blue,blue,1); //normalize(worldPosition),1.0);
}