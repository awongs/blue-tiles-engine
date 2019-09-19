#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	programsCreated = std::vector<GLuint>();
	shadersCreated = std::vector<GLuint>();
}

ShaderManager::~ShaderManager()
{
	// TODO delete compiled shaders

	// TODO delete shader programs
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

	shadersCreated.push_back(shader);

	return shader;
}

GLuint ShaderManager::CreateShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
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
		DebugLog::Info("Successfully combined and linked shader program\n");
	}


	return shaderProgram;
}

void ShaderManager::UseShaderProgram(GLuint shaderProgramID)
{
	glUseProgram(shaderProgramID);
	currentProgram = shaderProgramID;

	bool hasProgram = false;

	for (int i = 0; i < programsCreated.size() ; i++)
	{
		if (programsCreated[i] == shaderProgramID)
		{
			hasProgram = true;
			break;
		}
	}

	if (!hasProgram)
	{
		programsCreated.push_back(shaderProgramID);
	}
}
