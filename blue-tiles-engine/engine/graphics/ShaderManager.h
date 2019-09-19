#pragma once

#include <glad/glad.h>

#include "../debugbt/DebugLog.h";
#include <vector>

class ShaderManager
{

public:

	// Creates a shader manager
	ShaderManager();

	// Deletes the shader manager and cleans up shaders
	~ShaderManager();

	// Compiles a shader from the given shader code
	GLuint CompileShader(const char* shaderCode);

	// Creates and links up a shader program with two shaders
	GLuint CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

	// Selects the shader to be used for rendering
	void UseShaderProgram(GLuint shaderProgramID);

private:

	// List of shaders that was compiled by this shader manager
	std::vector<GLuint> shadersCreated;

	// List of shader programs created and linked by thise shader manager
	std::vector<GLuint> programsCreated;

	// The current ID of the shader program being used
	GLuint currentProgram;

	// 512 char error buffer used to store error messages when doing shader stuff
	char* errorBuffer;

};