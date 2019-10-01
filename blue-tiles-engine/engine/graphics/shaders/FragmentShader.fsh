#version 330 core

uniform sampler2D uTexture;

in vec2 fragTexCoord;

void main()
{
	gl_FragColor = vec4(fragTexCoord, 0.0, 1.0);
}