#version 330 core

#define MAX_JOINTS 64
#define MAX_WEIGHTS 3

// Inputs
layout (location = 0) in vec3 aPos;
layout (location = 3) in ivec3 aJointId;
layout (location = 4) in vec3 aJointWeight;

layout (std140) uniform Camera {
    uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 lightSpace;
};

// Camera matrices
uniform mat4 model;
//uniform mat4 lightSpace;

// Animation
uniform mat4 jointTransforms[MAX_JOINTS];

void main() 
{
    vec4 totalPosition = vec4(0.0);

	for (int i = 0; i < MAX_WEIGHTS; i++) 
	{
		mat4 jointTransform = jointTransforms[aJointId[i]];
		vec4 posePosition = jointTransform * vec4(aPos, 1.0);

		totalPosition += posePosition * aJointWeight[i];
	}

    gl_Position = lightSpace * model * vec4(totalPosition.xyz, 1.0);
}
