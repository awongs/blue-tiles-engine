#version 330 core

uniform sampler2D uTexture;

in vec2 fragTexCoord;
in vec3 fragNormal;

void main()
{
	// -- Testing -- Combine normal and texture for colour
	gl_FragColor = vec4(fragNormal, 1.0) + texture(uTexture, fragTexCoord);
}