#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec3 fragPosition;
out vec3 fragNormal;

void main()
{
	fragPosition = (model * vec4(aPos, 0.0)).xyz;
	fragNormal = (model * vec4(aNormal, 0.0)).xyz;
}