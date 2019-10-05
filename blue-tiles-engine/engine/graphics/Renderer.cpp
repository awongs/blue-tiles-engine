#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderManager.h"

#include "../debugbt/DebugLog.h"
#include "../../util/FileManager.h"
#include "Camera.h"
#include "../Scene.h"
#include "../behaviours/MeshRenderer.h"
#include "Texture.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: m_context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	m_shaderManager = new ShaderManager();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	DebugLog::Info("Setting up frame buffers \n");
	SetupBuffers();

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	DebugLog::Info("Renderer initialization completed!\n");
}

Renderer::~Renderer()
{
	// cleanup context
	SDL_GL_DeleteContext(m_context);
}

void Renderer::SetupShaders()
{
	// id of shader programs
	GLuint vertexShader;
	GLuint fragmentShader;

	// Load shader files into strings
	std::string deferredVertex = filemanager::LoadFile("../Assets/shaders/Deferred.vsh");
	std::string deferredFragment = filemanager::LoadFile("../Assets/shaders/Deferred.fsh");
	std::string defaultVertex = filemanager::LoadFile("../Assets/shaders/Default.vsh");
	std::string defaultFragment = filemanager::LoadFile("../Assets/shaders/Default.fsh");

	// Compile deferred shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredFragment.c_str());
	m_deferredShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile default shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, defaultVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, defaultFragment.c_str());
	m_defaultShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	m_shaderManager->UseShaderProgram(m_defaultShader.lock()->GetProgramHandle());
}

void Renderer::SetupBuffers()
{
	// TODO: Hardcoded width and height
	int screenWidth = 800;
	int screenHeight = 600;

	// Create the gBuffer
	glGenFramebuffers(1, &m_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

	// Position color buffer
	glGenTextures(1, &m_gPosition);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

	// Normal color buffer
	glGenTextures(1, &m_gNormal);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

	// Set color attachments for fragment shader
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, attachments);

	// Check for errors
	if (glGetError() != GL_NO_ERROR || glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		DebugLog::Warn("Error in SetupBuffers()");
	}

	// Go back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Render(Scene& currentScene)
{
	// clearing screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::shared_ptr<Shader> currentShader = m_shaderManager->GetCurrentShader().lock();

	// Make sure that the shader isn't null
	if (currentShader == nullptr)
	{
		DebugLog::Error("Pointer to current shader is null in render loop");
		return;
	}

	// Camera matrices
	glm::mat4 viewMatrix = Camera::GetInstance().GetViewMatrix();
	glm::mat4 projectionMatrix = Camera::GetInstance().GetProjectionMatrix();

	// Set camera matrices in shader
	currentShader->SetUniformMatrix4fv("view", viewMatrix);
	currentShader->SetUniformMatrix4fv("projection", projectionMatrix);

	for (const std::unique_ptr<GameObject>& gameObject : currentScene.getWorldGameObjects())
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		// Translate
		modelMatrix = glm::translate(modelMatrix, gameObject->position);

		// Rotate
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.x, glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.y, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, gameObject->rotation.z, glm::vec3(0, 0, 1));

		// Scale
		modelMatrix = glm::scale(modelMatrix, gameObject->scale);

		// Tell the game object to draw
		currentShader->SetUniformMatrix4fv("model", modelMatrix);
		gameObject->Draw();
	}
}

void Renderer::Display()
{
}
