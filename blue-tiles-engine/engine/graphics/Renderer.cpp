#include <memory>

#include "Renderer.h"
#include "ShaderManager.h"

#include "../debugbt/DebugLog.h"
#include "../../util/FileManager.h"
#include "Camera.h"

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

	// Load shader files into strings
	std::string vertexSource = filemanager::LoadFile("engine/graphics/shaders/VertexShader.vsh");
	std::string fragmentSource = filemanager::LoadFile("engine/graphics/shaders/FragmentShader.fsh");

	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());

	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesCube), m_verticesCube, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBufferObjectID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indicesCube), m_indicesCube, GL_STATIC_DRAW);

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

	std::shared_ptr<Shader> currentShader = m_shaderManager->GetCurrentShader().lock();

	// Make sure that the shader isn't null
	if (currentShader == nullptr)
	{
		DebugLog::Error("Pointer to current shader is null in render loop");
	}

	// MVP matrices, TODO: Model matrix is identity for now
	glm::mat4 modelMatrix = glm::mat4(1);
	glm::mat4 viewMatrix = Camera::GetInstance()->GetViewMatrix();
	glm::mat4 projectionMatrix = Camera::GetInstance()->GetProjectionMatrix();

	// Set matrices in shader
	currentShader->SetUniformMatrix4fv("model", modelMatrix);
	currentShader->SetUniformMatrix4fv("view", viewMatrix);
	currentShader->SetUniformMatrix4fv("projection", projectionMatrix);
	
	// draw functions
	glBindVertexArray(m_vertexAttributeObjectID);
	glDrawElements(GL_TRIANGLES, sizeof(m_indicesCube), GL_UNSIGNED_INT, 0);
}

void Renderer::Display()
{
}
