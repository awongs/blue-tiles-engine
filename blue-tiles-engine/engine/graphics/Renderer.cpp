#include <memory>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Texture.h"
#include "GeometryBuffer.h"
#include "ShadowBuffer.h"
#include "../debugbt/DebugLog.h"
#include "../Scene.h"
#include "../behaviours/MeshRenderer.h"
#include "../GameObject.h"
#include "../behaviours/DirectionalLight.h"
#include "../behaviours/PointLight.h"
#include "../behaviours/SpotLight.h"
#include "../../util/FileManager.h"

// Define maximum light count here. Should match the shader's maximums.
constexpr unsigned int MAX_POINT_LIGHTS = 256;
constexpr unsigned int MAX_SPOT_LIGHTS = 64;

Renderer::Renderer(SDL_GLContext* targetContext, int windowWidth, int windowHeight)
	: m_context(targetContext)
	, m_width(windowWidth)
	, m_height(windowHeight)
{
	DebugLog::Info("Renderer initialization starting...\n");

	m_shaderManager = new ShaderManager();

	DebugLog::Info("Creating shader program...\n");
	SetupShaders();

	// Create the geometry buffer for deferred rendering
	m_geometryBuffer = std::make_unique<GeometryBuffer>(windowWidth, windowHeight);

	// Create the shadow buffer, assumes that the width of the window is larger than its height
	m_shadowBuffer = std::make_unique<ShadowBuffer>(windowWidth, windowWidth);

	// Create the screen quad
	m_screenQuad = std::make_unique<GameObject>("screenQuad", glm::vec3(), glm::vec3(0, glm::pi<float>(), glm::pi<float>()));
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
	std::string shadowVertex = filemanager::LoadFile("../Assets/shaders/ShadowShader.vsh");
	std::string shadowFragment = filemanager::LoadFile("../Assets/shaders/ShadowShader.fsh");
	std::string deferredGeometryVertex = filemanager::LoadFile("../Assets/shaders/DeferredGeometryPass.vsh");
	std::string deferredGeometryFragment = filemanager::LoadFile("../Assets/shaders/DeferredGeometryPass.fsh");
	std::string transparencyVertex = filemanager::LoadFile("../Assets/shaders/TransparencyPass.vsh");
	std::string transparencyFragment = filemanager::LoadFile("../Assets/shaders/TransparencyPass.fsh");
	std::string deferredLightingVertex = filemanager::LoadFile("../Assets/shaders/DeferredLightingPass.vsh");
	std::string deferredLightingFragment = filemanager::LoadFile("../Assets/shaders/DeferredLightingPass.fsh");

	// Compile shadow shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, shadowVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, shadowFragment.c_str());
	m_shadowShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile geometry shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredGeometryVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredGeometryFragment.c_str());
	m_deferredGeometryShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile ozma shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, transparencyVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, transparencyFragment.c_str());
	m_transparencyShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile lighting shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, deferredLightingVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, deferredLightingFragment.c_str());
	m_deferredLightingShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Setup the uniform buffer
	glGenBuffers(1, &m_uniformBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PLight) * MAX_POINT_LIGHTS + sizeof(SLight) * MAX_SPOT_LIGHTS, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uniformBufferObject);
	
	// Bind the lighting uniform block to the uniform buffer
	unsigned int lights_index = glGetUniformBlockIndex(m_deferredLightingShader->GetProgramHandle(), "LightBlock");
	glUniformBlockBinding(m_deferredLightingShader->GetProgramHandle(), lights_index, 0);

	// Unbind for now
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::SetupLighting(Scene& currentScene)
{
	// Clear current pointers
	m_directionalLight.reset();
	m_pointLights.clear();
	m_spotLights.clear();

	// Loop through the scene game objects to find all lights
	for (const std::unique_ptr<GameObject>& gameObject : currentScene.GetWorldGameObjects())
	{
		// Use this directional light if there isn't already one set
		std::weak_ptr<DirectionalLight> dirLight = gameObject->GetBehaviour<DirectionalLight>();
		if (m_directionalLight.expired() && !dirLight.expired())
		{
			m_directionalLight = dirLight;
			continue;
		}

		// Get all point lights
		std::weak_ptr<PointLight> pointLight = gameObject->GetBehaviour<PointLight>();
		if (!pointLight.expired() && m_pointLights.size() < MAX_POINT_LIGHTS)
		{
			m_pointLights.push_back(pointLight);
			continue;
		}

		// Get all spot lights
		std::weak_ptr<SpotLight> spotLight = gameObject->GetBehaviour<SpotLight>();
		if (!spotLight.expired() && m_spotLights.size() < MAX_SPOT_LIGHTS)
		{
			m_spotLights.push_back(spotLight);
		}
	}
}

void Renderer::ShadowPass(Scene& currentScene)
{
	// Bind the shadow buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer->GetBufferID());

	// Set viewport to shadow map dimensions
	glViewport(0, 0, m_shadowBuffer->GetWidth(), m_shadowBuffer->GetHeight());

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Clear screen and depth buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render only back faces, this avoids self shadowing
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Use shadow shader
	m_shaderManager->UseShaderProgram(m_shadowShader->GetProgramHandle());

	// Do shadow mapping if there is a directional light
	if (!m_directionalLight.expired())
	{
		std::shared_ptr<DirectionalLight> dirLight = m_directionalLight.lock();
		glm::mat4 lightSpaceMatrix = dirLight->GetLightSpaceMatrix();

		// Translate by the camera's current position
		lightSpaceMatrix *= glm::translate(glm::mat4(1), -glm::round(Camera::GetInstance().GetPosition()));
		m_shadowShader->SetUniformMatrix4fv("lightSpace", lightSpaceMatrix);
	}

	// Draw the world
	currentScene.DrawWorld(*m_shadowShader);

	// Disable culling and reset face setting
	// If we end up not using quads for the level floor, keep this enabled for performance
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Renderer::GeometryPass(Scene& currentScene)
{
	// Bind the geometry buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_geometryBuffer->GetBufferID());

	// Set viewport to window dimensions
	glViewport(0, 0, m_width, m_height);

	// clearing screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use geometry shader
	m_shaderManager->UseShaderProgram(m_deferredGeometryShader->GetProgramHandle());

	// Do shadow mapping if there is a directional light
	if (!m_directionalLight.expired())
	{
		std::shared_ptr<DirectionalLight> dirLight = m_directionalLight.lock();
		glm::mat4 lightSpaceMatrix = dirLight->GetLightSpaceMatrix();

		// Translate by the camera's current position
		lightSpaceMatrix *= glm::translate(glm::mat4(1), -glm::round(Camera::GetInstance().GetPosition()));
		m_deferredGeometryShader->SetUniformMatrix4fv("lightSpace", lightSpaceMatrix);
		m_deferredGeometryShader->SetUniform3f("lightDirection", dirLight->GetDirection());
	}

	// Set camera matrices in shader
	m_deferredGeometryShader->SetUniformMatrix4fv("view", Camera::GetInstance().GetViewMatrix());
	m_deferredGeometryShader->SetUniformMatrix4fv("projection", Camera::GetInstance().GetProjectionMatrix());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer->GetDepthTexture());
	m_deferredGeometryShader->SetUniform1i("uShadowMap", 1);

	// Draw the world
	currentScene.DrawWorld(*m_deferredGeometryShader);
}

void Renderer::TransparencyPass(Scene& currentScene) {
	// Bind the geometry buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_geometryBuffer->GetBufferID());

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use transparency shader
	m_shaderManager->UseShaderProgram(m_transparencyShader->GetProgramHandle());

	// Set camera matrices in shader
	m_transparencyShader->SetUniformMatrix4fv("view", Camera::GetInstance().GetViewMatrix());
	m_transparencyShader->SetUniformMatrix4fv("projection", Camera::GetInstance().GetProjectionMatrix());

	// Draw all transparent objects
	for (const std::unique_ptr<GameObject>& worldGameObj : currentScene.GetWorldGameObjects()) {
		if (Camera::GetInstance().IsWithinBoundingBox(worldGameObj->position))
		{
			std::weak_ptr<MeshRenderer> meshRenderer = std::static_pointer_cast<MeshRenderer>(worldGameObj->GetBehaviour(BehaviourType::MeshRenderer).lock());

			if (!meshRenderer.expired() && meshRenderer.lock()->IsTransparent())
			{
				worldGameObj->Draw(*m_transparencyShader);
			}
		}
	}

	// Disable blending
	glDisable(GL_BLEND);
}

void Renderer::Render(Scene& currentScene)
{
	ShadowPass(currentScene);
	GeometryPass(currentScene);
	TransparencyPass(currentScene);

	// Write depth buffer from geometry pass to main frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_geometryBuffer->GetBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	// Set samplers in shader to their respective texture IDs
	m_deferredLightingShader->SetUniform1i("gPosition", 0);
	m_deferredLightingShader->SetUniform1i("gNormal", 1);
	m_deferredLightingShader->SetUniform1i("gColour", 2);

	// Set camera position
	glm::vec3 position = Camera::GetInstance().GetPosition();
	position.z = -position.z;
	m_deferredLightingShader->SetUniform3f("cameraPosition", position);

	// Render directional light if it's not null
	if (!m_directionalLight.expired())
	{
		m_directionalLight.lock()->Render(*m_deferredLightingShader);
	}

	// Bind the uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_uniformBufferObject);

	int pointLightCount = 0;
	int spotLightcount = 0;

	// Loop backwards since lights may be removed from the list
	for (int i = m_pointLights.size() - 1; i >= 0; i--)
	{
		if (m_pointLights[i].expired())
		{
			m_pointLights.erase(m_pointLights.begin() + i);
		}
		else
		{
			std::shared_ptr<PointLight> pointLight = m_pointLights[i].lock();
			
			// Only render lights that are within the camera's bounding box
			// Note: Does not consider light radius, but that's fine for almost all lights
			if (Camera::GetInstance().IsWithinBoundingBox(pointLight->gameObject->position))
			{
				pointLight->Render(*m_deferredLightingShader,
				sizeof(PLight) * pointLightCount++);
			}
		}
	}

	// Loop backwards since lights may be removed from the list
	for (int i = m_spotLights.size() - 1; i >= 0; i--)
	{
		if (m_spotLights[i].expired())
		{
			m_spotLights.erase(m_spotLights.begin() + i);
		}
		else
		{
			std::shared_ptr<SpotLight> spotLight = m_spotLights[i].lock();

			// Only render lights that are within the camera's bounding box
			// Note: Does not consider light radius, but that's fine for almost all lights
			if (Camera::GetInstance().IsWithinBoundingBox(spotLight->gameObject->position))
			{
				spotLight->Render(*m_deferredLightingShader,
					sizeof(PLight) * MAX_POINT_LIGHTS + sizeof(SLight) * spotLightcount++);
			}
		}
	}

	// Set light counts in shader
	m_deferredLightingShader->SetUniform1i("totalPointLights", pointLightCount);
	m_deferredLightingShader->SetUniform1i("totalSpotLights", spotLightcount);

	// Draw onto the quad
	m_screenQuad->Draw(*m_deferredLightingShader);

	// Disable depth test and unbind UBO
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	currentScene.DrawScreen(*m_deferredGeometryShader);
}
