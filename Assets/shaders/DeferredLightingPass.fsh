#version 330 core

layout (location = 0) out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColour;

in vec2 fragTexCoord;

void main()
{
	// Sample geometry from textures
	vec4 position = texture(gPosition, fragTexCoord);
	vec4 normal = texture(gNormal, fragTexCoord);
	vec4 colour = texture(gColour, fragTexCoord);

	// -- Testing -- Mash everything together for now
	fragColor = position * normal * colour;
}