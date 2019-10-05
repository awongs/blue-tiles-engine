#version 330 core

layout (location = 0) out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

in vec2 fragTexCoord;

void main()
{
	fragColor = (texture(gNormal, fragTexCoord)) + vec4(0.0, 0.1, 0.2, 1.0);
	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}