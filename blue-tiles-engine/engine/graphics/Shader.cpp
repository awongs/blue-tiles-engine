#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "..//debugbt/DebugLog.h"


Shader::Shader(GLuint programHandle)
	: m_programHandle(programHandle)
{
}

Shader::~Shader()
{
	// Tell OpenGL that we no longer need this shader
	glDeleteShader(m_programHandle);
}

void Shader::SetUniform1f(std::string name, float value)
{
	int id = glGetUniformLocation(m_programHandle, name.c_str());

	if (id != -1)
	{
		// Set the value
		glUniform1f(id, value);
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform float named: " + name);
	}
}

void Shader::SetUniform2f(std::string name, glm::vec2 values)
{
	int id = glGetUniformLocation(m_programHandle, name.c_str());

	if (id != -1)
	{
		// Set the values
		glUniform2f(id, values.x, values.y);
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform vec2 named: " + name);
	}
}

void Shader::SetUniform3f(std::string name, glm::vec3 values)
{
	int id = glGetUniformLocation(m_programHandle, name.c_str());

	if (id != -1)
	{
		// Set the values
		glUniform3f(id, values.x, values.y, values.z);
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform vec3 named: " + name);
	}
}

void Shader::SetUniform4f(std::string name, glm::vec4 values)
{
	int id = glGetUniformLocation(m_programHandle, name.c_str());

	if (id != -1)
	{
		// Set the values
		glUniform4f(id, values.x, values.y, values.z, values.w);
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform vec4 named: " + name);
	}
}

void Shader::SetUniformMatrix4fv(std::string name, glm::mat4& matrix)
{
	int id = glGetUniformLocation(m_programHandle, name.c_str());

	if (id != -1)
	{
		// Set the matrix
		glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform matrix named: " + name);
	}
}
