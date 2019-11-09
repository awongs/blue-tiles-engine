#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include "../animation/AnimatedMesh.h"


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

	// Delete uniform buffers.
	glDeleteBuffers(1, &m_lightUniformBuffer);
	glDeleteBuffers(1, &m_cameraUniformBuffer);
	glDeleteBuffers(1, &m_animationUniformBuffer);

	delete m_shaderManager;
}

void Renderer::SetupShaders()
{
	// id of shader programs
	GLuint vertexShader;
	GLuint fragmentShader;

	// Load shader files into strings
	std::string shadowVertex = FileManager::LoadFile("../Assets/shaders/Shadow.vsh");
	std::string shadowFragment = FileManager::LoadFile("../Assets/shaders/Shadow.fsh");
	std::string animationShadowVertex = FileManager::LoadFile("../Assets/shaders/AnimationShadow.vsh");
	std::string animationShadowFragment = FileManager::LoadFile("../Assets/shaders/AnimationShadow.fsh");
	std::string geometryVertex = FileManager::LoadFile("../Assets/shaders/Geometry.vsh");
	std::string geometryFragment = FileManager::LoadFile("../Assets/shaders/Geometry.fsh");
	std::string animationGeometryVertex = FileManager::LoadFile("../Assets/shaders/AnimationGeometry.vsh");
	std::string animationGeometryFragment = FileManager::LoadFile("../Assets/shaders/AnimationGeometry.fsh");
	std::string transparencyVertex = FileManager::LoadFile("../Assets/shaders/Transparency.vsh");
	std::string transparencyFragment = FileManager::LoadFile("../Assets/shaders/Transparency.fsh");
	std::string lightingVertex = FileManager::LoadFile("../Assets/shaders/Lighting.vsh");
	std::string lightingFragment = FileManager::LoadFile("../Assets/shaders/Lighting.fsh");

	// Compile shadow shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, shadowVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, shadowFragment.c_str());
	m_shadowShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile animation shadow shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, animationShadowVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, animationShadowFragment.c_str());
	m_aniShadowShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile geometry shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, geometryVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, geometryFragment.c_str());
	m_geometryShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile animation geometry shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, animationGeometryVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, animationGeometryFragment.c_str());
	m_aniGeometryShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile transparency shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, transparencyVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, transparencyFragment.c_str());
	m_transparencyShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Compile lighting shader
	vertexShader = m_shaderManager->CompileShader(GL_VERTEX_SHADER, lightingVertex.c_str());
	fragmentShader = m_shaderManager->CompileShader(GL_FRAGMENT_SHADER, lightingFragment.c_str());
	m_lightingShader = m_shaderManager->CreateShaderProgram(vertexShader, fragmentShader);

	// Setup the light uniform buffer
	glGenBuffers(1, &m_lightUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_lightUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PLight) * MAX_POINT_LIGHTS + sizeof(SLight) * MAX_SPOT_LIGHTS, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_lightUniformBuffer);

	// Bind the lighting uniform block to the uniform buffer
	int index = glGetUniformBlockIndex(m_lightingShader->GetProgramHandle(), "LightBlock");
	glUniformBlockBinding(m_lightingShader->GetProgramHandle(), index, 0);

	// Setup the camera uniform buffer
	glGenBuffers(1, &m_cameraUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_cameraUniformBuffer);

	// Bind the camera uniform block to the uniform buffer for all applicable shaders
	index = glGetUniformBlockIndex(m_aniShadowShader->GetProgramHandle(), "Camera");
	glUniformBlockBinding(m_aniShadowShader->GetProgramHandle(), index, 1);

	index = glGetUniformBlockIndex(m_shadowShader->GetProgramHandle(), "Camera");
	glUniformBlockBinding(m_shadowShader->GetProgramHandle(), index, 1);

	index = glGetUniformBlockIndex(m_geometryShader->GetProgramHandle(), "Camera");
	glUniformBlockBinding(m_geometryShader->GetProgramHandle(), index, 1);

	index = glGetUniformBlockIndex(m_aniGeometryShader->GetProgramHandle(), "Camera");
	glUniformBlockBinding(m_aniGeometryShader->GetProgramHandle(), index, 1);

	index = glGetUniformBlockIndex(m_transparencyShader->GetProgramHandle(), "Camera");
	glUniformBlockBinding(m_transparencyShader->GetProgramHandle(), index, 1);

	// Setup the animation uniform buffer
	glGenBuffers(1, &m_animationUniformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_animationUniformBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 64, NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_animationUniformBuffer);

	index = glGetUniformBlockIndex(m_aniShadowShader->GetProgramHandle(), "Animation");
	glUniformBlockBinding(m_aniShadowShader->GetProgramHandle(), index, 2);

	index = glGetUniformBlockIndex(m_aniGeometryShader->GetProgramHandle(), "Animation");
	glUniformBlockBinding(m_aniGeometryShader->GetProgramHandle(), index, 2);

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

	// Draw the world
	for (auto& worldGameObj : currentScene.GetWorldGameObjects())
	{
		// Only draw objects that are within the camera's view
		// Note: Does not consider model size
		if (Camera::GetInstance().IsWithinBoundingBox(worldGameObj->position))
		{
			std::weak_ptr<MeshRenderer> meshRenderer = std::static_pointer_cast<MeshRenderer>(worldGameObj->GetBehaviour(BehaviourType::MeshRenderer).lock());
			std::weak_ptr<AnimatedMesh> animatedMesh = std::static_pointer_cast<AnimatedMesh>(worldGameObj->GetBehaviour(BehaviourType::AnimatedMesh).lock());

			// Don't draw transparent objects
			if (!meshRenderer.expired() && meshRenderer.lock()->IsTransparent())
			{
				continue;
			}

			if (!animatedMesh.expired())
			{
				m_shaderManager->UseShaderProgram(m_aniShadowShader->GetProgramHandle());
				animatedMesh.lock()->BindJointTransforms(m_animationUniformBuffer);
				worldGameObj->Draw(*m_aniShadowShader);
			}
			else
			{
				m_shaderManager->UseShaderProgram(m_shadowShader->GetProgramHandle());
				worldGameObj->Draw(*m_shadowShader);
			}
		}
	}

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

	// Do shadow mapping if there is a directional light
	if (!m_directionalLight.expired())
	{
		std::shared_ptr<DirectionalLight> dirLight = m_directionalLight.lock();
		m_shaderManager->UseShaderProgram(m_geometryShader->GetProgramHandle());
		m_geometryShader->SetUniform3f("lightDirection", dirLight->GetDirection());
		
		m_shaderManager->UseShaderProgram(m_aniGeometryShader->GetProgramHandle());
		m_aniGeometryShader->SetUniform3f("lightDirection", dirLight->GetDirection());
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowBuffer->GetDepthTexture());
	
	// Set uniforms in shader
	m_shaderManager->UseShaderProgram(m_geometryShader->GetProgramHandle());
	m_geometryShader->SetUniform1i("uShadowMap", 1);

	m_shaderManager->UseShaderProgram(m_aniGeometryShader->GetProgramHandle());
	m_aniGeometryShader->SetUniform1i("uShadowMap", 1);

	// Draw the world
	//currentScene.DrawWorld(*m_deferredGeometryShader);
	for (auto& worldGameObj : currentScene.GetWorldGameObjects())
	{
		// Only draw objects that are within the camera's view
		// Note: Does not consider model size
		if (Camera::GetInstance().IsWithinBoundingBox(worldGameObj->position))
		{
			// TODO: Improve this
			std::weak_ptr<MeshRenderer> meshRenderer = std::static_pointer_cast<MeshRenderer>(worldGameObj->GetBehaviour(BehaviourType::MeshRenderer).lock());
			std::weak_ptr<AnimatedMesh> animatedMesh = std::static_pointer_cast<AnimatedMesh>(worldGameObj->GetBehaviour(BehaviourType::AnimatedMesh).lock());
			
			// Don't draw transparent objects
			if (!meshRenderer.expired() && meshRenderer.lock()->IsTransparent())
			{
				continue;
			}

			if (!animatedMesh.expired())
			{
				// Use animation shader.
				m_shaderManager->UseShaderProgram(m_aniGeometryShader->GetProgramHandle());
				animatedMesh.lock()->BindJointTransforms(m_animationUniformBuffer);
				worldGameObj->Draw(*m_aniGeometryShader);
			}
			else
			{
				// Use geometry shader.
				m_shaderManager->UseShaderProgram(m_geometryShader->GetProgramHandle());
				worldGameObj->Draw(*m_geometryShader);
			}
		}
	}
}

void Renderer::TransparencyPass(Scene& currentScene) {
	// Bind the geometry buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_geometryBuffer->GetBufferID());

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use transparency shader
	m_shaderManager->UseShaderProgram(m_transparencyShader->GetProgramHandle());

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
	// Update camera matrices in the uniform buffer.
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Camera::GetInstance().GetViewMatrix()));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Camera::GetInstance().GetProjectionMatrix()));

	// Do shadow mapping if there is a directional light
	if (!m_directionalLight.expired())
	{
		std::shared_ptr<DirectionalLight> dirLight = m_directionalLight.lock();
		glm::mat4 lightSpaceMatrix = dirLight->GetLightSpaceMatrix();

		// Translate by the camera's current position
		lightSpaceMatrix *= glm::translate(glm::mat4(1), -glm::round(Camera::GetInstance().GetPosition()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
	m_shaderManager->UseShaderProgram(m_lightingShader->GetProgramHandle());

	// Set samplers in shader to their respective texture IDs
	m_lightingShader->SetUniform1i("gPosition", 0);
	m_lightingShader->SetUniform1i("gNormal", 1);
	m_lightingShader->SetUniform1i("gColour", 2);

	// Set camera position
	glm::vec3 position = Camera::GetInstance().GetPosition();
	position.z = -position.z;
	m_lightingShader->SetUniform3f("cameraPosition", position);

	// Render directional light if it's not null
	if (!m_directionalLight.expired())
	{
		m_directionalLight.lock()->Render(*m_lightingShader);
	}

	// Bind the light uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, m_lightUniformBuffer);

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
				pointLight->Render(*m_lightingShader,
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
				spotLight->Render(*m_lightingShader,
					sizeof(PLight) * MAX_POINT_LIGHTS + sizeof(SLight) * spotLightcount++);
			}
		}
	}

	// Set light counts in shader
	m_lightingShader->SetUniform1i("totalPointLights", pointLightCount);
	m_lightingShader->SetUniform1i("totalSpotLights", spotLightcount);

	// Draw onto the quad
	m_screenQuad->Draw(*m_lightingShader);

	// Disable depth test and unbind UBO
	glDisable(GL_DEPTH_TEST);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
