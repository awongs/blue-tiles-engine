#include "Renderer.h"
#include "ShaderManager.h"

#include "../debugbt/DebugLog.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: m_context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	DebugLog::Info("Generating buffers...\n");
	SetupBuffers();

	m_shaderManager = new ShaderManager();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	DebugLog::Info("Renderer initialization completed!\n");
}

Renderer::~Renderer()
{
	// cleanup context
	SDL_GL_DeleteContext(m_context);

	glDeleteBuffers(1, &m_vertexBufferObjectID);
}

void Renderer::SetupShaders()
{
	// id of shader programs
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, m_vertexSource);

	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, m_fragmentSource);

	shaderProgram = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	m_shaderManager->UseShaderProgram(shaderProgram);

	glBindFragDataLocation(shaderProgram, 0, "FragColor");

	// vertex data layout
	GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Renderer::SetupBuffers()
{		
	// generate buffers
	glGenVertexArrays(1, &m_vertexAttributeObjectID);
	glGenBuffers(1, &m_vertexBufferObjectID);
	glGenBuffers(1, &m_indicesBufferObjectID);


	// bind the Vertex Array Object first, then bind and set Vertex Buffers and attribute pointers
	glBindVertexArray(m_vertexAttributeObjectID);

	// copy vertex data to vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	// declare that position is at index 0 of attribs for VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// clear buffer bindings
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // do not unbind.
	glBindVertexArray(0);
}

void Renderer::Render()
{
	// clearing screen
	glClearColor(0.75f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw functions
	glBindVertexArray(m_vertexAttributeObjectID);
	glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);
}

void Renderer::Display()
{
}
