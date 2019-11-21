#version 330 core

#define MAXNUMBEROFLIGHTS 100
#define POINTLIGHTSHADOWSIZE 1024
#define DIRECTIONALLIGHTSHADOWSIZE 4096
//16384

struct PointLight {    
    vec3 position;
    float scrap;
    vec3 colour;
    float radius;
};

struct DirectionalLight {    
    vec3 direction;
    float scrap;
    vec3 colour;
    float scrap2;
};

layout (std140) uniform PointLightData {
    int numLights;
    PointLight pointLights[MAXNUMBEROFLIGHTS];
} allPointLights;

layout (std140) uniform DirectionalLightData {
    int numLights;
    DirectionalLight directionalLights[MAXNUMBEROFLIGHTS];
} allDirectionalLights;

layout (std140) uniform DirectionalLightInfo {
    mat4 directionalLightMatricies[MAXNUMBEROFLIGHTS];
    int shadowSize;
} allDiretcionalLightMatrices;

layout (std140) uniform PointLightInfo {
    mat4 pointLightMatricies[MAXNUMBEROFLIGHTS];
    int shadowSize;
} allPointLightMatrices;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D shadowTex;
uniform samplerCube cubeTex;

uniform vec4 nodeColour;

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    vec3 worldPos;
    vec3 cameraPosition;
} IN;

out vec4 fragColour;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 samplePos);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 samplePos, int index);

float DirectionalShadowCalculation(int index, vec2 offset) {
    // perform perspective divide
    vec4 fragPosLightSpace = allDiretcionalLightMatrices.directionalLightMatricies[index] * vec4(IN.worldPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    projCoords.xy *= 1.0 / float(allDirectionalLights.numLights);

    if (index == 1) {
        projCoords.x += 0.5;//(index * DIRECTIONALLIGHTSHADOWSIZE) / allLightMatrices.shadowSize;
    }
    //projCoords.x += (index * DIRECTIONALLIGHTSHADOWSIZE) / allLightMatrices.shadowSize;
    //projCoords.y += (index * DIRECTIONALLIGHTSHADOWSIZE) % allLightMatrices.shadowSize;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    projCoords.x += offset.x;
    projCoords.y += offset.y;
    float closestDepth = texture(shadowTex, projCoords.xy, 0).r;//texture(, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.005 * (1.0 - dot(IN.normal, normalize(allDirectionalLights.directionalLights[index].direction - IN.worldPos))), 0.005);

    return currentDepth - 0.005 > closestDepth ? 0.0 : 1.0; //Used 0.25 for a softer shadow
}

float GetBlurredShadowValue(int index) {
    float texMovementAmmount = allDiretcionalLightMatrices.shadowSize;
    texMovementAmmount = 1.0 / texMovementAmmount;

    float combinedValue = 0.0;
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            vec2 offset;
            offset.x = i * texMovementAmmount;
            offset.y = j * texMovementAmmount;
            //combinedValue += texture(shadowTex, samplePosition).r;
            combinedValue += DirectionalShadowCalculation(index, offset);
        }
    }
    return (combinedValue / 25.0);
}

void main (void) {
    vec2 samplePosition = vec2(IN.texCoord.x, 1f - IN.texCoord.y);
    vec4 diffuse = texture ( diffuseTex, samplePosition);// * material.ambient;

    if (diffuse.w == 0) {
        return;
    }

    mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
    vec3 normal = normalize((texture(normalTex, samplePosition).rgb * 2.0 - 1.0));

    vec3 viewDir = normalize(IN.cameraPosition - IN.worldPos);

    vec3 result = vec3(0 ,0 ,0);
    for (int i = 0; i < min(allPointLights.numLights, MAXNUMBEROFLIGHTS); ++i) {
        result += CalculatePointLight(allPointLights.pointLights[i], normal, IN.worldPos, viewDir, samplePosition);
    }
    for (int i = 0; i < min(allDirectionalLights.numLights, MAXNUMBEROFLIGHTS); ++i) {
        result += CalculateDirectionalLight(allDirectionalLights.directionalLights[i], normal, IN.worldPos, viewDir, samplePosition, i);
    }

    fragColour = vec4(result * nodeColour.xyz, nodeColour.w);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 samplePos) {
    vec3 lightDir = normalize(light.position - fragPos);
    float dist = length(light.position - fragPos);

    if (dist > light.radius)
        return vec3(0,0,0);


    //diffuse shading ammount
    float diff = max(dot(normal, lightDir), 0.0);

    //specular shading ammount
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfVector), 0.0), 33.0);//material.shininess);

    float shadow = 1.0;// - ShadowCalculation();

    //Attenuation
    float attenuation = 1.0 - clamp (dist / light.radius, 0.0, 1.s0);

    vec3 ambient  = vec3(texture(diffuseTex, samplePos));
    vec3 diffuse  = diff * vec3(texture(diffuseTex, samplePos));
    vec3 specular = vec3(spec, spec, spec);// * vec3(texture(material.specular, TexCoords)); light.specular * 
    return ((ambient + diffuse + specular) * attenuation * light.colour.rgb * shadow);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 samplePos, int index) {
    vec3 lightDir = light.direction;

    //diffuse shading ammount
    float diff = max(dot(normal, lightDir), 0.0);

    //specular shading ammount
    vec3 halfVector = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfVector), 0.0), 33.0);//material.shininess);

    float shadow = GetBlurredShadowValue(index);//DirectionalShadowCalculation(index);

    vec3 ambient  = vec3(texture(diffuseTex, samplePos));
    vec3 diffuse  = diff * vec3(texture(diffuseTex, samplePos));
    vec3 specular = vec3(spec, spec, spec);// * vec3(texture(material.specular, TexCoords)); light.specular * 

    return ((ambient + diffuse + specular) * light.colour.rgb * shadow);
}