#include "Renderer.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	DebugLog::Info("Generating buffers...\n");
	SetupBuffers();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	DebugLog::Info("Renderer initialization completed!\n");
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
	char buffer[512];

	// Vertex shader
	DebugLog::Info("Compiling vertex shader");

	// create shader object
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// compile vertex shader
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// get status code
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &statusCode);

	if (statusCode == GL_FALSE)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		DebugLog::Error(buffer);
	}
	else
	{
		DebugLog::Info("Successfully compiled vertex shader\n");
	}

	// Fragment Shader
	DebugLog::Info("Compiling fragment shader");

	// create shader object
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// compile fragment shader
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// get status code
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusCode);

	if (statusCode == GL_FALSE)
	{
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
	shaderProgram = glCreateProgram();

	// attach shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// error code
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &statusCode);
	if (statusCode == GL_FALSE)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(buffer), NULL, buffer);
		DebugLog::Error(buffer);
	}
	else
	{
		DebugLog::Info("Successfully combined and linked shader program\n");
	}

	// vertex data layout
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::SetupBuffers()
{
	GLfloat vertices[] = {
	 0.0f,  0.5f, // Vertex 1 (X, Y)
	 0.5f, -0.5f, // Vertex 2 (X, Y)
	-0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	GLenum statusCode;

	// Generate 1 buffer
	glGenBuffers(1, &vbo);

	statusCode = glGetError();
	if (statusCode != 0) DebugLog::Error("Failed to generate buffers! " + std::to_string(statusCode));
	
	// bind buffer to gpu
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	statusCode = glGetError();
	if (statusCode != 0) DebugLog::Error("Failed to bind buffers! " + std::to_string(statusCode));
	
	// copy vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	statusCode = glGetError();
	if (statusCode != 0) DebugLog::Error("Failed to code to buffers! " + std::to_string(statusCode));
}

void Renderer::Render()
{
	// Clearing screen
	glClearColor(0.75f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::Display()
{
}
