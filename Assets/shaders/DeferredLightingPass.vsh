#version 330 core

uniform mat4 model;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 fragTexCoord;

void main()
{
	fragTexCoord = aTexCoord;
	gl_Position = model * vec4(aPos, 1.0);
}