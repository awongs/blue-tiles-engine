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

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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

	// create game engine
	GameEngine* engine = new GameEngine(window);

	SDL_Event windowEvent;

	// Empty loop to prevent the window from closing immediately.
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		if (windowEvent.type == SDL_KEYUP &&
			windowEvent.key.keysym.sym == SDLK_ESCAPE) break;

		engine->Update();

		engine->Draw();
	}

	std::cout << "End of engine life." << std::endl;

	// destroy engine
	delete engine;
	engine = NULL;

	// Clean up SDL.
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}