#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_mixer.h>
#include <iostream>

#include "engine/GameEngine.h"
#include "engine/input/Input.h"
#include "engine/debugbt/DebugLog.h"

#include "engine/sound/Sound.h"
#include "engine/sound/Music.h"
#include "engine/sound/SoundManager.h"

int main()
{
	GameWindow gameWin(800, 600);

	int windowSetupStatus = gameWin.SetupSDLWindow();

	if (windowSetupStatus)
	{
		return 1;
	}

	

	// create game engine
	GameEngine* engine = new GameEngine(gameWin.GetWindow());

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
	
	return 0;
}