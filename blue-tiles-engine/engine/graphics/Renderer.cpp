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

	// Create the screen quad
	std::vector<std::unique_ptr<Behaviour>> behaviours;
	behaviours.push_back(std::make_unique<MeshRenderer>(420, "../Assets/models/quad.obj"));
	screenQuad = std::make_unique<GameObject>(behaviours);

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
	std::string deferredGeometryVertex = filemanager::LoadFile("../Assets/shaders/DeferredGeometryPass.vsh");
	std::string deferredGeometryFragment = filemanager::LoadFile("../Assets/shaders/DeferredGeometryPass.fsh");
	std::string deferredLightingVertex = filemanager::LoadFile("../Assets/shaders/DeferredLightingPass.vsh");
	std::string deferredLightingFragment = filemanager::LoadFile("../Assets/shaders/DeferredLightingPass.fsh");

	// Compile deferred shaders
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredGeometryVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredGeometryFragment.c_str());
	m_deferredGeometryShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredLightingVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredLightingFragment.c_str());
	m_deferredLightingShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	m_shaderManager->UseShaderProgram(m_deferredGeometryShader.lock()->GetProgramHandle());
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

	// Attach depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

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
	

	glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
	// clearing screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// -- Testing -- Point the camera diagonally towards the origin
	Camera::GetInstance().SetPosition(glm::vec3(0.0f, 20.0f, 20.0f));
	Camera::GetInstance().SetOrientation(glm::vec3(glm::quarter_pi<float>(), 0.0f, 0.0f));

	std::shared_ptr<Shader> geometryShader = m_deferredGeometryShader.lock();
	m_shaderManager->UseShaderProgram(geometryShader->GetProgramHandle());

	// Make sure that the shader isn't null
	if (geometryShader == nullptr)
	{
		DebugLog::Error("Pointer to current shader is null in render loop");
		return;
	}

	// Set camera matrices in shader
	geometryShader->SetUniformMatrix4fv("view", Camera::GetInstance().GetViewMatrix());
	geometryShader->SetUniformMatrix4fv("projection", Camera::GetInstance().GetProjectionMatrix());
 
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
		geometryShader->SetUniformMatrix4fv("model", modelMatrix);
		gameObject->Draw();
	}
}

void Renderer::Display()
{
	// Bind the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);

	// Switch to lighting shader
	std::shared_ptr<Shader> lightingShader = m_deferredLightingShader.lock();
	m_shaderManager->UseShaderProgram(lightingShader->GetProgramHandle());

	// Make sure that the shader isn't null
	if (lightingShader == nullptr)
	{
		DebugLog::Error("Pointer to current shader is null in render loop");
		return;
	}

	// -- Testing -- Point the camera diagonally towards the origin
	Camera::GetInstance().SetPosition(glm::vec3(0.0f, 0.0f, 0.8f));
	Camera::GetInstance().SetOrientation(glm::vec3(0.0f, 0.0f, 0.0f));

	// Set camera matrices in shader
	lightingShader->SetUniformMatrix4fv("view", Camera::GetInstance().GetViewMatrix());
	lightingShader->SetUniformMatrix4fv("projection", Camera::GetInstance().GetProjectionMatrix());

	glm::mat4 modelMatrix = glm::mat4(1);

	// Translate
	modelMatrix = glm::translate(modelMatrix, screenQuad->position);

	// Rotate
	modelMatrix = glm::rotate(modelMatrix, screenQuad->rotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 0, 1));

	// Scale
	modelMatrix = glm::scale(modelMatrix, screenQuad->scale);

	lightingShader->SetUniform1i("gNormal", 1);
	glBindTexture(GL_TEXTURE_2D, m_gNormal);

	lightingShader->SetUniformMatrix4fv("model", modelMatrix);

	// Draw 
	screenQuad->Draw();
}
