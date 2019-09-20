	#pragma once

#include <sdl2\SDL.h>
#include <glad/glad.h>

class ShaderManager;

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
	SDL_GLContext* m_context;

	// Pointer to a shader manager
	ShaderManager* m_shaderManager;

	// ID to the vertex buffer object
	GLuint m_vertexBufferObjectID;

	// ID to the vertex array object
	GLuint m_vertexAttributeObjectID;

	// ID to the indices buffer object
	GLuint m_indicesBufferObjectID;

	// place holder triangle vertices

	GLfloat m_vertices[12] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int m_indices[6] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	// place holder Shader code
	const char* m_vertexSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* m_fragmentSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
		"}\n\0";

};