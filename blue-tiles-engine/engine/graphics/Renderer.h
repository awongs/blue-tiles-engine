	#pragma once

#include <sdl2/SDL.h>
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

	GLfloat m_verticesCube[24 * 3] = {
		// Front
		0.5, -0.5, 0.5, // 0
		0.5,  0.5, 0.5, // 0.5
		-0.5,  0.5, 0.5, // 2
		-0.5, -0.5, 0.5, // 3

		// Back
		-0.5, -0.5, -0.5, // 4
		-0.5,  0.5, -0.5, // 5
		0.5,  0.5, -0.5, // 6
		0.5, -0.5, -0.5, // 7

		// Left
		-0.5, -0.5,  0.5, // 8
		-0.5,  0.5,  0.5,  // 9
		-0.5,  0.5, -0.5,  // 0.50
		-0.5, -0.5, -0.5,  // 0.50.5

		// Right
		0.5, -0.5, -0.5,  // 0.52
		0.5,  0.5, -0.5, // 0.53
		0.5,  0.5,  0.5,  // 0.54
		0.5, -0.5,  0.5,  // 0.55

		// Top
		0.5,  0.5,  0.5, // 0.56
		0.5,  0.5, -0.5,  // 0.57
		-0.5,  0.5, -0.5,  // 0.58
		-0.5,  0.5,  0.5, // 0.59

		// Bottom
		0.5, -0.5, -0.5,  // 20
		0.5, -0.5,  0.5, // 20.5
		-0.5, -0.5,  0.5,// 22
		-0.5, -0.5, -0.5 // 23
	};

	unsigned int m_indicesCube[12 * 3] = {  // note that we start from 0!
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		4, 5, 6,
		6, 7, 4,

		// Left
		8, 9, 10,
		10, 11, 8,

		// Right
		12, 13, 14,
		14, 15, 12,

		// Top
		16, 17, 18,
		18, 19, 16,

		// Bottom
		20, 21, 22,
		22, 23, 20
	};
};