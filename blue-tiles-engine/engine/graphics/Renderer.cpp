#include "Renderer.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...");

	SetupShaders();

	DebugLog::Info("Renderer initialization completed!");
}

Renderer::~Renderer()
{
	// Cleanup context
	SDL_GL_DeleteContext(context);

	glDeleteBuffers(1, &vbo);
}

void Renderer::SetupShaders()
{
	// compile status code
	GLint statusCode;

	// Vertex shader
	DebugLog::Info("Compiling vertex shader");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// compile vertex shader
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// get status code
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &statusCode);

	if (statusCode == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

		DebugLog::Error(buffer);
	}
	else
	{
		DebugLog::Info("Successfully compiled vertex shader\n");
	}

	// Fragment Shader
	DebugLog::Info("Compiling fragment shader");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// compile fragment shader
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// get status code
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusCode);

	if (statusCode == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);

		DebugLog::Error(buffer);
	}
	else
	{
		DebugLog::Info("Successfully compiled fragment shader\n");
	}

	// combine shader into a program
	DebugLog::Info("Combning and linking shader program");

	// create shader program instance
	GLuint shaderProgram = glCreateProgram();

	// attach shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);

	//glUseProgram(shaderProgram);

	// error code
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &statusCode);
	if (statusCode == GL_FALSE)
	{
		char buffer[512];
		glGetProgramInfoLog(shaderProgram, sizeof(buffer), NULL, buffer);

		DebugLog::Error(buffer);
	}
	else
	{
		DebugLog::Info("Successfully combined and linked shader program\n");
	}
}

void Renderer::SetupBuffers()
{
	glGenBuffers(1, &vbo); // Generate 1 buffer
}

void Renderer::Render()
{

	float vertices[] = {
	 0.0f,  0.5f, // Vertex 1 (X, Y)
	 0.5f, -0.5f, // Vertex 2 (X, Y)
	-0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

void Renderer::Display()
{
}
