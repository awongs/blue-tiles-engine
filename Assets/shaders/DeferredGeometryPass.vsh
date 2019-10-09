#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main()
{
	fragPosition = (model * vec4(aPos, 1.0)).xyz;
	fragTexCoord = aTexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	fragNormal = normalMatrix * aNormal;

	gl_Position = projection * view * vec4(fragPosition, 1.0);
}