#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "..//debugbt/DebugLog.h"


Shader::Shader(GLuint programHandle)
	: m_programHandle(programHandle)
{
}

Shader::~Shader()
{
	DebugLog::Info("Cleaning up Shader #" + std::to_string(m_programHandle));

	// Tell OpenGL that we no longer need this shader
	glDeleteShader(m_programHandle);
}

GLuint Shader::GetProgramHandle() const
{
	return m_programHandle;
}

void Shader::SetUniform1i(const std::string& name, const int value)
{
	int id = getUniformLocation(name);

	if (id != -1)
	{
		// Set the value
		glUniform1i(id, value);
	}
	else
	{
		DebugLog::Error("Couldn't find a uniform int named: " + name);
	}
}

void Shader::SetUniform1f(const std::string& name, const float value)
{
	int id = getUniformLocation(name);

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

void Shader::SetUniform2f(const std::string& name, const glm::vec2 values)
{
	int id = getUniformLocation(name);

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

void Shader::SetUniform3f(const std::string& name, const glm::vec3 values)
{
	int id = getUniformLocation(name);

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

void Shader::SetUniform4f(const std::string& name, const glm::vec4 values)
{
	int id = getUniformLocation(name);

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

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
{
	int id = getUniformLocation(name);

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

int Shader::getUniformLocation(const std::string& uniformName)
{
	// Check if this uniform is in the cache.
	if (m_uniformMap.count(uniformName) == 0)
	{
		m_uniformMap[uniformName] = glGetUniformLocation(m_programHandle, uniformName.c_str());
	}

	return m_uniformMap.at(uniformName);
}
