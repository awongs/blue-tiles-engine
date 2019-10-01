#version 330 core

uniform sampler2D uTexture;

in vec2 fragTexCoord;

void main()
{
	gl_FragColor = texture(uTexture, fragTexCoord);
}