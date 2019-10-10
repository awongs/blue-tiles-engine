#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gColour;

uniform sampler2D uTexture;
uniform sampler2D uShadowMap;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec4 fragLightSpacePosition; // Shadow map coordinates

float calcShadow();

void main()
{
	gPosition = fragPosition;
	gNormal = normalize(fragNormal);
	gColour = texture(uTexture, fragTexCoord).rgb * calcShadow();
}

lowp float calcShadow() 
{
    // Convert shadow map coordinates into uv format (0 to 1)
    vec3 projCoords = fragLightSpacePosition.xyz / fragLightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;
    
    // Calculate depths
    float closestDepth = texture2D(uShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    float bias = 0.005;
    return currentDepth - bias < closestDepth ? 1.0 : 0.3; // 1.0 means no shadow
}