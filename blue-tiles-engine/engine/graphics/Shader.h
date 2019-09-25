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
	
	// Sets a uniform float value in the shader.
	void SetUniform1f(std::string name, float value);

	// Sets a uniform vector with two values in the shader.
	void SetUniform2f(std::string name, glm::vec2 values);

	// Sets a uniform vector with three values in the shader.
	void SetUniform3f(std::string name, glm::vec3 values);

	// Sets a uniform vector with four values in the shader.
	void SetUniform4f(std::string name, glm::vec4 values);

	// Sets a uniform 4x4 matrix in the shader.
	void SetUniformMatrix4fv(std::string name, glm::mat4 &matrix);

private:
	// The program number of the compiled shader.
	GLuint m_programHandle;

	// Prevent copy construction.
	Shader(const Shader& other);
};