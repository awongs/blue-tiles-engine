#version 330 core

// Inputs
layout (location = 0) in vec3 aPos;

layout (std140) uniform Camera {
    uniform mat4 view;
	uniform mat4 projection;
	uniform mat4 lightSpace;
};

// Camera matrices
uniform mat4 model;
//uniform mat4 lightSpace;

void main() 
{
    gl_Position = lightSpace * model * vec4(aPos, 1.0);
}
