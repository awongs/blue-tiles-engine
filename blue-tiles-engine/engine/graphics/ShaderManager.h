#pragma once

#include <glad/glad.h>
#include <vector>
#include <memory>

#include "Shader.h"

class ShaderManager
{

public:

	// Creates a shader manager
	ShaderManager();

	// Deletes the shader manager and cleans up shaders
	~ShaderManager();

	// Compiles a shader from the given shader code
	GLuint CompileShader(GLuint shaderType, const char* shaderCode);

	// Creates and links up a shader program with two shaders
	std::shared_ptr<Shader> CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);

	// Selects the shader to be used for rendering
	void UseShaderProgram(GLuint shaderProgramID);

	// Accessor for the current shader.
	std::shared_ptr<Shader> GetCurrentShader() const;

private:

	// List of shaders that was compiled by this shader manager
	std::vector<GLuint> m_shadersCreated;

	// List of shader programs created and linked by thise shader manager
	std::vector<std::shared_ptr<Shader>> m_programsCreated;

	// Pointer to the current ID of the shader program being used
	std::shared_ptr<Shader> m_currentShader;
};