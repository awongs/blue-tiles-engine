#pragma once

#include <sdl2/SDL.h>
#include <glad/glad.h>


class GeometryBuffer;
class MeshRenderer;
class ShaderManager;
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

	// Renders the frame
	void Render(Scene& currentScene);

	// Displays the frame to the context
	void Display();

private:

	// SDL context for the render target
	SDL_GLContext* m_context;

	// Pointer to a shader manager
	ShaderManager* m_shaderManager;

	// Pointer to the geometry buffer.
	std::unique_ptr<GeometryBuffer> m_geometryBuffer;

	// Pointer to the deferred geometry shader.
	std::shared_ptr<Shader> m_deferredGeometryShader;

	// Pointer to the deferred lighting shader.
	std::shared_ptr<Shader> m_deferredLightingShader;

	// Quad to render the frame onto.
	std::unique_ptr<MeshRenderer> m_screenQuad;
};