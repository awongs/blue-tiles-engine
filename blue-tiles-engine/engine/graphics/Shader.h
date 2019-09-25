#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	// Constructor, takes a program handle parameter.
	Shader(GLuint programHandle);

	// Destructor.
	~Shader();

	GLuint GetProgramHandle();
	
	// Sets a uniform float value in the shader.
	void SetUniform1f(const std::string name, const float value) const;

	// Sets a uniform vector with two values in the shader.
	void SetUniform2f(const std::string name, const glm::vec2 values) const;

	// Sets a uniform vector with three values in the shader.
	void SetUniform3f(const std::string name, const glm::vec3 values) const;

	// Sets a uniform vector with four values in the shader.
	void SetUniform4f(const std::string name, const glm::vec4 values) const;

	// Sets a uniform 4x4 matrix in the shader.
	void SetUniformMatrix4fv(const std::string name, const glm::mat4 &matrix) const;

private:
	// The program number of the compiled shader.
	GLuint m_programHandle;

	// Prevent copy construction.
	Shader(const Shader& other);
};