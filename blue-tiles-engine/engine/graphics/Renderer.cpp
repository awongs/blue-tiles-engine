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
#include "ShadowBuffer.h"
#include "../GameObject.h"
#include "../behaviours/DirectionalLight.h"
#include "../behaviours/PointLight.h"
#include "../behaviours/SpotLight.h"

Renderer::Renderer(SDL_GLContext* targetContext)
	: m_context(targetContext)
{
	DebugLog::Info("Renderer initialization starting...\n");

	m_shaderManager = new ShaderManager();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	// TODO: Hardcoded screen width and height
	m_geometryBuffer = std::make_unique<GeometryBuffer>(800, 600);
	m_shadowBuffer = std::make_unique<ShadowBuffer>(800, 600);

	// Create the screen quad
	m_screenQuad = std::make_unique<GameObject>(1337, "screenQuad", glm::vec3(), glm::vec3(0, glm::pi<float>(), glm::pi<float>()));
	m_screenQuad->AddBehaviour(new MeshRenderer("../Assets/models/quad.obj"));

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

void Renderer::ShadowPass(Scene& currentScene)
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}

void Renderer::GeometryPass(Scene& currentScene)
{
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

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
 
	// Draw the world
	currentScene.DrawWorld(*m_deferredGeometryShader);
  
	// Disable depth testing
	glDisable(GL_DEPTH_TEST);
}

void Renderer::Render(Scene& currentScene)
{
	ShadowPass(currentScene);
	GeometryPass(currentScene);

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

	// Set samplers in shader to their respective texture IDs
	m_deferredLightingShader->SetUniform1i("gPosition", 0);
	m_deferredLightingShader->SetUniform1i("gNormal", 1);
	m_deferredLightingShader->SetUniform1i("gColour", 2);
	
	// Set camera position
	glm::vec3 position = Camera::GetInstance().GetPosition();
	position.z = -position.z;
	m_deferredLightingShader->SetUniform3f("cameraPosition", position);

	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;

	// Unoptimized, looping through all objects in scene to find lights
	for (const std::unique_ptr<GameObject>& light : currentScene.GetWorldGameObjects())
	{
		DirectionalLight* dirLight = static_cast<DirectionalLight*>(light->GetBehaviour(BehaviourType::DirectionalLight));

		if (dirLight != nullptr)
		{
			dirLight->Render(*m_deferredLightingShader);
			continue;
		}

		PointLight* pointLight = static_cast<PointLight*>(light->GetBehaviour(BehaviourType::PointLight));

		if (pointLight != nullptr)
		{
			pointLights.push_back(pointLight);
		}

		SpotLight* spotLight = static_cast<SpotLight*>(light->GetBehaviour(BehaviourType::SpotLight));

		if (spotLight != nullptr)
		{
			spotLights.push_back(spotLight);
		}
	}

	// Set point light count in shader
	m_deferredLightingShader->SetUniform1i("totalPointLights", pointLights.size());
	m_deferredLightingShader->SetUniform1i("totalSpotLights", spotLights.size());

	for (int i = 0; i < pointLights.size(); i++)
	{
		pointLights[i]->Render(*m_deferredLightingShader, i);
	}

	for (int i = 0; i < spotLights.size(); i++)
	{
		spotLights[i]->Render(*m_deferredLightingShader, i);
	}

	// Draw onto the quad
	m_screenQuad->Draw(*m_deferredLightingShader);
}
