#include "ShaderManager.h"

#include "../debugbt/DebugLog.h"

ShaderManager::ShaderManager()
{
	m_programsCreated = std::vector<std::shared_ptr<Shader>>();
	m_shadersCreated = std::vector<GLuint>();
}

ShaderManager::~ShaderManager()
{
	DebugLog::Info("Cleaning up ShaderManager");
	for (GLuint shader : m_shadersCreated)
	{
		glDeleteShader(shader);
	}
}

GLuint ShaderManager::CompileShader(GLuint shaderType, const char* shaderCode)
{
	// compile status code
	GLint statusCode;
	char errorBuffer[512];

	// create shader object
	GLuint shader = glCreateShader(shaderType);

	// compile vertex shader
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	// get status code
	glGetShaderiv(shader, GL_COMPILE_STATUS, &statusCode);

	if (statusCode != GL_TRUE)
	{
		DebugLog::Error("Compile Error Code: " + std::to_string(statusCode));
		glGetShaderInfoLog(shader, sizeof(errorBuffer), NULL, errorBuffer);
		DebugLog::Error(errorBuffer);
	}
	else
	{
		DebugLog::Info("Successfully compiled shader (" + std::to_string(shader) + ")");
	}

	m_shadersCreated.push_back(shader);

	return shader;
}

std::shared_ptr<Shader> ShaderManager::CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	// compile status code
	GLint statusCode;
	char errorBuffer[512];

	GLuint shaderProgram = glCreateProgram();

	// attach shaders
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// error code
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &statusCode);
	if (statusCode != GL_TRUE)
	{
		DebugLog::Error("Linker Error Code: " + std::to_string(statusCode));
		glGetProgramInfoLog(shaderProgram, sizeof(errorBuffer), NULL, errorBuffer);
		DebugLog::Error(errorBuffer);
	}
	else
	{
		DebugLog::Info("Successfully combined and linked shader program #" + std::to_string(shaderProgram));
	}

	// Wrap program ID into Shader class
	std::shared_ptr<Shader> newShader = std::make_shared<Shader>(shaderProgram);
	m_programsCreated.push_back(newShader);

	return newShader;
}

void ShaderManager::UseShaderProgram(GLuint shaderProgramID)
{
	// Find the compiled shader
	for (const std::shared_ptr<Shader>& shader : m_programsCreated)
	{
		if (shader->GetProgramHandle() == shaderProgramID)
		{
			// Set current shader
			m_currentShader = shader;
			glUseProgram(shaderProgramID);
			return;
		}
	}

	// Couldn't find a shader program with the specified ID
	DebugLog::Warn("Couldn't find Shader #" + std::to_string(shaderProgramID));
}

std::shared_ptr<Shader> ShaderManager::GetCurrentShader() const
{
	return m_currentShader;
}
