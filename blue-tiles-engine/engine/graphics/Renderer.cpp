#include "Renderer.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	DebugLog::Info("Generating buffers...\n");
	SetupBuffers();

	shaderManager = new ShaderManager();

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
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	vertexShader = shaderManager->CompileShader(vertexSource);

	fragmentShader = shaderManager->CompileShader(fragmentSource);

	shaderProgram = shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	shaderManager->UseShaderProgram(shaderProgram);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	// vertex data layout
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::SetupBuffers()
{		
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	// Bind the Vertex Array Object first, then bind and set Vertex Buffers and attribute pointers
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // ?
	glBindVertexArray(0);
}

void Renderer::Render()
{
	// Clearing screen
	glClearColor(0.75f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw functions
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 9);
	glBindVertexArray(0);
}

void Renderer::Display()
{
}
