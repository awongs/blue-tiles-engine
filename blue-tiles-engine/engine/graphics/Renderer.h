#pragma once

#include <sdl2\SDL.h>
#include <glad/glad.h>

#include "../debugbt/DebugLog.h"

/*
	This renders the game engine.
*/
class Renderer
{
public:

	Renderer(SDL_GLContext* targetContext);

	~Renderer();

	void SetupShaders();

	void SetupBuffers();

	void Render();

	void Display();

private:

	SDL_GLContext* context;

	GLuint vertexShader;

	GLuint fragmentShader;

	GLuint shaderProgram;

	GLuint vbo;

	GLuint vao;

	GLfloat vertices[9] = { 0.0f, -0.5f, 0.0f,
					   -0.5f, 0.5f, 0.0f,
						0.5f, 0.5f, 0.0f };

	// Shader code
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