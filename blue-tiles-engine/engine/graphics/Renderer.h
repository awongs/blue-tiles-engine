	#pragma once

#include <sdl2\SDL.h>
#include <glad/glad.h>

#include "../debugbt/DebugLog.h"
#include "ShaderManager.h"

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
	void Render();

	// Displays the frame to the context
	void Display();

private:

	// SDL context for the render target
	SDL_GLContext* context;

	// pointer to a shader manager
	ShaderManager* shaderManager;

	// ID to the vertex buffer object
	GLuint vertexBufferObjectID;

	// ID to the vertex array object
	GLuint vertexAttributeObjectID;

	// ID to the indices buffer object
	GLuint indicesBufferObjectID;

	// place holder triangle vertices

	GLfloat vertices[12] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[6] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	// place holder Shader code
	const char* vertexSource = R"glsl(
		#version 150 core

		in vec2 position;

		void main()
		{
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";

	const char* fragmentSource = R"glsl(
		#version 150 core

		out vec4 outColor;

		void main()
		{
			outColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
	)glsl";

};