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

	GLuint m_uniformBufferObject;

	// SDL context for the render target
	SDL_GLContext* m_context;

	// Pointer to a shader manager
	ShaderManager* m_shaderManager;

	// Pointer to the shadow buffer.
	std::unique_ptr<ShadowBuffer> m_shadowBuffer;

	// Pointer to the geometry buffer.
	std::unique_ptr<GeometryBuffer> m_geometryBuffer;

	// Pointer to the shadow shader.
	std::shared_ptr<Shader> m_shadowShader;

	// Pointer to the deferred geometry shader.
	std::shared_ptr<Shader> m_deferredGeometryShader;

	// Pointer to the transparency shader.
	std::shared_ptr<Shader> m_transparencyShader;

	// Pointer to the deferred lighting shader.
	std::shared_ptr<Shader> m_deferredLightingShader;

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