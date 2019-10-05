#version 330 core

uniform sampler2D uTexture;

in vec2 fragTexCoord;
in vec3 fragNormal;

layout (location = 0) out vec4 fragColor;

void main()
{
	// -- Testing -- Combine normal and texture for colour
	fragColor = vec4(fragNormal, 1.0) + texture(uTexture, fragTexCoord);
}