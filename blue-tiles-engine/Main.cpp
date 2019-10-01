#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_mixer.h>
#include <iostream>

#include "engine/GameEngine.h"
#include "engine/input/Input.h"
#include "engine/debugbt/DebugLog.h"
#include "engine/sound/Music.h"
#include "engine/sound/SoundManager.h"

int main()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << 
			std::endl;
		return 1;
	}

	

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

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

	// Create the input manager.
	Input *input{ new Input() };

	SDL_Event windowEvent;

	// Create a sound manager
	SoundManager* soundManager = new SoundManager();
	
	//Shared pointer to a music object.
	auto music = soundManager->getMusic("alarm");

	music->play();
	// Empty loop to prevent the window from closing immediately.
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;

			// Update the inputs.
			input->HandleInput(windowEvent);
		}

		// Quit the game with cancel key.
		if (input->IsKeyDown(Input::INPUT_CANCEL))
			break;

		// TODO: Input testing. Remove this later.
		if (input->IsKeyDown(Input::INPUT_UP))
			DebugLog::Info("Up key pressed.");
		if (input->IsKeyDown(Input::INPUT_DOWN))
			DebugLog::Info("Down key pressed.");
		if (input->IsKeyDown(Input::INPUT_LEFT))
			DebugLog::Info("Left key pressed.");
		if (input->IsKeyDown(Input::INPUT_RIGHT))
			DebugLog::Info("Right key pressed.");
		if (input->IsMouseButtonDown(SDL_BUTTON_LEFT))
			DebugLog::Info("Left mouse button pressed.");
		if (input->IsMouseButtonDown(SDL_BUTTON_RIGHT))
			DebugLog::Info("Right mouse button pressed.");
		if (input->IsMouseButtonDown(SDL_BUTTON_MIDDLE))
			DebugLog::Info("Middle mouse button pressed.");

		engine->Update();

		engine->Draw();
	}

	std::cout << "End of engine life." << std::endl;

	// Delete the input manager.
	delete input;
	input = nullptr;

	// destroy engine
	delete engine;
	engine = NULL;

	// destroy SoundManager
	delete soundManager;
	soundManager = NULL;
	// Clean up SDL.
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}