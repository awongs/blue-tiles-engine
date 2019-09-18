#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>

#include <iostream>

#include "engine/GameEngine.h"

int main()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << 
			std::endl;
		return 1;
	}

	// Create the window.
	SDL_Window *window{ nullptr };
	window = SDL_CreateWindow(
		"Blue Tiles Engine",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		800,
		600,
		SDL_WINDOW_OPENGL);

	// Check if the window was created.
	if (window == nullptr)
	{
		std::cout << "Window could not be created: " << SDL_GetError() <<
			std::endl;
		return 1;
	}

	GameEngine* engine = new GameEngine();

	// Empty loop to prevent the window from closing immediately.
	while (true)
	{

	}

	// Clean up SDL.
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}