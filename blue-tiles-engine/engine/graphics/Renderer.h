#pragma once

#include <sdl2/SDL.h>
#include <glad/glad.h>
#include <vector>

#include "../behaviours/PointLight.h"

class GeometryBuffer;
class ShadowBuffer;
class GameObject;
class ShaderManager;
class Shader;
class Scene;
class DirectionalLight;
class PointLight;
class SpotLight;

/*
	This renders the game engine.
*/
class Renderer
{
public:

	// Creates a renderer that renders to the target context
	Renderer(SDL_GLContext* targetContext, int windowWidth, int windowHeight);

	// Deletes the renderer and deletes it's own shader manager
	~Renderer();

	// Sets up shaders for rendering
	void SetupShaders();

	// Updates the lists of lights.
	void SetupLighting(Scene& currentScene);

	// Performs a shadow pass by rendering objects and storing depth information.
	void ShadowPass(Scene& currentScene);

	// Performs a geometry pass in the deferred rendering process.
	void GeometryPass(Scene& currentScene);

	// Performs a transparency pass.
	void TransparencyPass(Scene& currentScene);

	// Renders the frame
	void Render(Scene& currentScene);

private:

	// Width and height of the screen.
	int m_width, m_height;

	// SDL context for the render target
	SDL_GLContext* m_context;

	// Uniform buffer object for storing camera matrices.
	GLuint m_cameraUniformBuffer;

	// Uniform buffer object for storing lights.
	GLuint m_lightUniformBuffer;

	// Uniform buffer object for animations.
	GLuint m_animationUniformBuffer;

	// Pointer to a shader manager
	ShaderManager* m_shaderManager;

	// Pointer to the shadow buffer.
	std::unique_ptr<ShadowBuffer> m_shadowBuffer;

	// Pointer to the geometry buffer.
	std::unique_ptr<GeometryBuffer> m_geometryBuffer;

	// Pointers to shaders.
	std::shared_ptr<Shader> m_shadowShader;
	std::shared_ptr<Shader> m_aniShadowShader;
	std::shared_ptr<Shader> m_geometryShader;
	std::shared_ptr<Shader> m_aniGeometryShader;
	std::shared_ptr<Shader> m_transparencyShader;
	std::shared_ptr<Shader> m_lightingShader;

	// Quad to render the frame onto.
	std::unique_ptr<GameObject> m_screenQuad;

	// List of all point lights in the scene.
	// Needs to be updated if more lights are added to the scene.
	std::vector<std::weak_ptr<PointLight>> m_pointLights;

	// List of all spot lights in the scene.
	// Needs to be updated if more lights are added to the scene.
	std::vector<std::weak_ptr<SpotLight>> m_spotLights;

	// The main directional light in the scene.
	std::weak_ptr<DirectionalLight> m_directionalLight;
};