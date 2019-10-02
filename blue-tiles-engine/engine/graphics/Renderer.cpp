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
	GLuint shaderProgram;

	// Load shader files into strings
	std::string vertexSource = filemanager::LoadFile("engine/graphics/shaders/VertexShader.vsh");
	std::string fragmentSource = filemanager::LoadFile("engine/graphics/shaders/FragmentShader.fsh");

	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());

	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

	shaderProgram = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	m_shaderManager->UseShaderProgram(shaderProgram);
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
