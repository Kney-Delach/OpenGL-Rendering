#version 330

out vec4 color;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float u_NearPlane;
uniform float u_FarPlane;
uniform bool IsDirectional;

// When dispalying a perspective projection matrix, this function linearized its data back to NDC
float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * u_NearPlane * u_FarPlane) / (u_FarPlane + u_NearPlane - z * (u_FarPlane - u_NearPlane));
}

void main()
{
	float depthValue = texture(depthMap, TexCoords).r;
	if(IsDirectional)
		color = texture(depthMap, TexCoords); // render orthographic texture, no need to normalize 
	else 
		color = vec4(vec3(LinearizeDepth(depthValue) / u_FarPlane), 1.0); // render perspective texture, normalize it back to ndc before displaying

}