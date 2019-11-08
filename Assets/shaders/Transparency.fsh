#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColour;

uniform sampler2D uTexture;
uniform sampler2D uShadowMap;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

void main()
{   
	gPosition = fragPosition;
	gNormal = normalize(fragNormal);
	gColour = texture(uTexture, fragTexCoord); //* calcShadow(lightDirection, gNormal);
}