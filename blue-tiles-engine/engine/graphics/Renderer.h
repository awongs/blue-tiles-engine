#pragma once

#include <sdl2/SDL.h>
#include <glad/glad.h>

class ShaderManager;
class GameObject;
class Shader;
class Scene;

/*
	This renders the game engine.
*/
class Renderer
{
public:

	// Creates a renderer that renders to the target context
	Renderer(SDL_GLContext* targetContext);

	// Deletes the renderer and deletes it's own shader manager
	~Renderer();

	// Sets up shaders for rendering
	void SetupShaders();

	// Sets up buffers for rendering
	void SetupBuffers();

	// Renders the frame
	void Render(Scene& currentScene);

	// Displays the frame to the context
	void Display();

private:

	// SDL context for the render target
	SDL_GLContext* m_context;

	// Pointer to a shader manager
	ShaderManager* m_shaderManager;

	// Pointer to the deferred geometry shader.
	std::weak_ptr<Shader> m_deferredGeometryShader;

	// Pointer to the deferred lighting shader.
	std::weak_ptr<Shader> m_deferredLightingShader;

	// Geometry frame buffer for deferred rendering.
	GLuint m_gBuffer;

	GLuint m_depthBuffer;
	
	// Position texture in geometry buffer.
	GLuint m_gPosition;

	// Normal texture in geometry buffer.
	GLuint m_gNormal;

	std::unique_ptr<GameObject> screenQuad;
};