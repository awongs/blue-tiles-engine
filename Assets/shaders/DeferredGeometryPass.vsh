#version 330 core

#define MAX_JOINTS 64
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

	/*
	// TODO: Clean this up
	vec4 totalPosition = vec4(0.0);
	vec4 totalNormal = vec4(0.0);

	for (int i = 0; i < MAX_WEIGHTS; i++) 
	{
		mat4 jointTransform = jointTransforms[aJointId[i]];
		vec4 posePosition = jointTransform * vec4(fragPosition, 1.0);
		vec4 poseNormal = jointTransform * vec4(fragNormal, 1.0);

		totalPosition += posePosition * aJointWeight[i];
		totalNormal += poseNormal * aJointWeight[i];
	}
	*/

	gl_Position = projection * view * vec4(fragPosition, 1.0);
}