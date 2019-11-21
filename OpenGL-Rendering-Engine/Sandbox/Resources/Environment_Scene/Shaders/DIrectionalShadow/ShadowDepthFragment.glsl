#version 330

out vec4 color;

void main()
{
	color = vec4(gl_FragCoord.z); // This line represents what is happening behind the scenes
}