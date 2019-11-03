#version 330 

uniform sampler2D u_DiffuseTexture;

in vec2 o_TexCoord; 
in vec4 o_Colour; 

out vec4 gl_FragColor;

// default values used in editor camera
float m_Near = 0.1; 
float m_Far = 100.0; 
  
float LinearizeDepth(float depth) 
{
	float z = depth * 2.0 - 1.0; // transform back to normalized device co-ordinates 
	return (2.0 * m_Near * m_Far) / (m_Far + m_Near - z * (m_Far - m_Near));	
}

void main()
{
	float depth = LinearizeDepth(gl_FragCoord.z) / m_Far; // divide by far for demonstration
	if(depth > 0.15) // if depth greater than 0.15, discard any fragments
	{
		discard;
	}
	gl_FragColor = vec4(vec3(depth), 1.0);
}