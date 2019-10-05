#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPosition;
out vec3 fragNormal;

void main()
{
	fragPosition = (model * vec4(aPos, 1.0)).xyz;
	fragNormal = aNormal;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}