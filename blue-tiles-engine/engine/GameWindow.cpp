#include "GameWindow.h"

#include <sdl2/SDL.h>
#include <iostream>

GameWindow::GameWindow(int width, int height)
	: m_windowHeight(height)
	, m_windowWidth(width)
{
	SetupSDLWindow();
}

GameWindow::~GameWindow()
{
	// Clean up SDL.
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void GameWindow::SetupSDLWindow()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() <<
			std::endl;
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window.
	m_window = SDL_CreateWindow(
		"Blue Tiles Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_windowWidth,
		m_windowHeight,
		SDL_WINDOW_OPENGL);

	// Check if the window was created.
	if (m_window == nullptr)
	{
		std::cout << "Window could not be created: " << SDL_GetError() <<
			std::endl;
		return;
	}
}
