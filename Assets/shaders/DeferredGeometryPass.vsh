#version 330 core

#define MAX_JOINTS 50
#define MAX_WEIGHTS 3

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in ivec3 aJointId;
layout (location = 4) in vec3 aJointWeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpace;

// Animation
uniform mat4 jointTransforms[MAX_JOINTS];

out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec4 fragLightSpacePosition;

void main()
{
	// Position and texture
	fragPosition = (model * vec4(aPos, 1.0)).xyz;
	fragTexCoord = aTexCoord;

	// Normal vector
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	fragNormal = normalMatrix * aNormal;

	// Shadow mapping coordinates
    fragLightSpacePosition = lightSpace * vec4(fragPosition, 1.0);

	gl_Position = projection * view * vec4(fragPosition, 1.0);
}