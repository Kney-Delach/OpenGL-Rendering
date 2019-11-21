# version 330 core

layout (std140) uniform Matrices {
    uniform mat4 projMatrix;
    uniform mat4 viewMatrix;
    uniform vec3 cameraPos;
};

uniform mat4 modelMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec3 cameraPosition;
} OUT;

void main (void) {
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    OUT.colour = colour;
    OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy; // NOT THE SAME 
    OUT.normal = normalize(normalMatrix * normalize (normal));
    OUT.tangent = normalize (normalMatrix * normalize (tangent));
    OUT.binormal = normalize (normalMatrix * normalize (cross (normal, tangent)));
    OUT.worldPos = vec3((modelMatrix * vec4(position, 1))); // same as FragPosition
    OUT.cameraPosition = cameraPos; //same
    gl_Position = (projMatrix * viewMatrix * modelMatrix ) * vec4 (position , 1.0); // same
}