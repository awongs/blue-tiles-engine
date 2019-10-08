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
#include "GeometryBuffer.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: m_context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	m_shaderManager = new ShaderManager();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	// TODO: Hardcoded screen width and height
	m_geometryBuffer = std::make_unique<GeometryBuffer>(800, 600);

	// Create the screen quad
	m_screenQuad = std::make_unique<MeshRenderer>(9001, "../Assets/models/quad.obj");

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	DebugLog::Info("Renderer initialization completed!\n");
}

Renderer::~Renderer()
{
	DebugLog::Info("Cleaning up Renderer");

	// cleanup context
	SDL_GL_DeleteContext(m_context);

	delete m_shaderManager;
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

	// Compile geometry shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredGeometryVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredGeometryFragment.c_str());
	m_deferredGeometryShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile lighting shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredLightingVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredLightingFragment.c_str());
	m_deferredLightingShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);
}

void Renderer::Render(Scene& currentScene)
{
	// Bind the geometry buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_geometryBuffer->GetBufferID());

	// clearing screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use geometry shader
	m_shaderManager->UseShaderProgram(m_deferredGeometryShader->GetProgramHandle());

	// Make sure that the shader isn't null
	if (m_deferredGeometryShader == nullptr)
	{
		DebugLog::Error("Pointer to geometry shader is null in render loop");
		return;
	}

	// Set camera matrices in shader
	m_deferredGeometryShader->SetUniformMatrix4fv("view", Camera::GetInstance().GetViewMatrix());
	m_deferredGeometryShader->SetUniformMatrix4fv("projection", Camera::GetInstance().GetProjectionMatrix());
 
	// Render the scene
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

		// Set model matrix in shader
		m_deferredGeometryShader->SetUniformMatrix4fv("model", modelMatrix);

		// Note: Not explicitly setting the value of uTexture since it's already at the correct value of 0

		// Tell the game object to draw
		gameObject->Draw();
	}
}

void Renderer::Display()
{
	// Bind the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind geometry textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_geometryBuffer->GetPositionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_geometryBuffer->GetNormalTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_geometryBuffer->GetColourTexture());

	// Switch to lighting shader
	m_shaderManager->UseShaderProgram(m_deferredLightingShader->GetProgramHandle());

	// Make sure that the shader isn't null
	if (m_deferredLightingShader == nullptr)
	{
		DebugLog::Error("Pointer to lighting shader is null in render loop");
		return;
	}

	// TODO - This never changes, can be optimized
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::pi<float>(), glm::vec3(0, 0, 1));

	// Set samplers in shader to their respective texture IDs
	m_deferredLightingShader->SetUniform1i("gPosition", 0);
	m_deferredLightingShader->SetUniform1i("gNormal", 1);
	m_deferredLightingShader->SetUniform1i("gColour", 2);

	// Set model matrix in shader
	m_deferredLightingShader->SetUniformMatrix4fv("model", modelMatrix);

	// Draw onto the quad
	m_screenQuad->Draw();
}
