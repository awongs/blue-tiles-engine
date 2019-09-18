#pragma once

#include <glad/glad.h>

#include "../debugbt/DebugLog.h";
#include <vector>

class ShaderManager
{

public:

	ShaderManager();

	~ShaderManager();

	GLuint CompileShader(const char* shaderCode);

	GLuint CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

	void UseShaderProgram(GLuint shaderProgramID);

private:

	std::vector<GLuint> programs;

	GLuint currentProgram;

	char* errorBuffer;

};