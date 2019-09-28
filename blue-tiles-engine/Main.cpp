#include <glad/glad.h>

#define SDL_MAIN_HANDLED
#include <sdl2/SDL.h>
#include <sdl2/SDL_mixer.h>
#include <iostream>

#include "engine/GameEngine.h"
#include "engine/physics/PhysicsEngine.h"
#include "engine/physics/Collider.h"
#include "engine/input/Input.h"
#include "engine/debugbt/DebugLog.h"
#include "engine/sound/Sound.h"

int main()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << 
			std::endl;
		return 1;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		std::cout << "Could not initialize SDL_mixer:" << Mix_GetError() <<
			std::endl;
		return 2;
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

	// Create the physics engine.
	PhysicsEngine *physics{ new PhysicsEngine() };

	// TODO: Collision-testing for physics engine, remove this later.
	glm::vec3 col1HalfSizes{ 2.f };
	Collider *col1{ new Collider(col1HalfSizes) };
	glm::vec3 col1Pos{ 0.f };
	col1->SetPosition(col1Pos);
	physics->AddPhysicsObject(0, col1, 
		[](const PhysicsObject &other) { });

	glm::vec3 col2HalfSizes{ 3.f };
	Collider *col2{ new Collider(col2HalfSizes) };
	glm::vec3 col2Pos{ 6.f, 0.f, 0.f };
	col2->SetPosition(col2Pos);
	physics->AddPhysicsObject(1, col2, 
		[](const PhysicsObject &other) { });

	// Create the input manager.
	Input *input{ new Input() };

	SDL_Event windowEvent;
	Sound *sound {new Sound()};
	const string path = "../Assets/sound-assets/Alert.mp3";
	sound->addMusic(path);
	
	/*if((Mix_PlayMusic(sound->getMusic(), -1))!=0) {
		DebugLog::Error("Failed");
	}*/
	
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

		// TODO: Input and collision testing. Remove this later.
		if (input->IsKeyDown(Input::INPUT_UP))
		{
			//DebugLog::Info("Up key pressed.");
			col1Pos.y += 0.01f;
			col1->SetPosition(col1Pos);
			DebugLog::Info("\n  col1: [x:" + std::to_string(col1Pos.x) +
				" y:" + std::to_string(col1Pos.y) +
				" w/2:" + std::to_string(col1HalfSizes.x) +
				" h/2:" + std::to_string(col1HalfSizes.y) +
				"], \n  col2: [x:" + std::to_string(col2Pos.x) +
				" y:" + std::to_string(col2Pos.y) +
				" w/2:" + std::to_string(col2HalfSizes.x) +
				" h/2:" + std::to_string(col2HalfSizes.y) + "]");
		}
		if (input->IsKeyDown(Input::INPUT_DOWN))
		{
			//DebugLog::Info("Down key pressed.");
			col1Pos.y -= 0.01f;
			col1->SetPosition(col1Pos);
			DebugLog::Info("\n  col1: [x:" + std::to_string(col1Pos.x) +
				" y:" + std::to_string(col1Pos.y) +
				" w/2:" + std::to_string(col1HalfSizes.x) +
				" h/2:" + std::to_string(col1HalfSizes.y) +
				"], \n  col2: [x:" + std::to_string(col2Pos.x) +
				" y:" + std::to_string(col2Pos.y) +
				" w/2:" + std::to_string(col2HalfSizes.x) +
				" h/2:" + std::to_string(col2HalfSizes.y) + "]");
		}
		if (input->IsKeyDown(Input::INPUT_LEFT))
		{
			//DebugLog::Info("Left key pressed.");
			col1Pos.x -= 0.01f;
			col1->SetPosition(col1Pos);
			DebugLog::Info("\n  col1: [x:" + std::to_string(col1Pos.x) + 
				" y:" + std::to_string(col1Pos.y) + 
				" w/2:" + std::to_string(col1HalfSizes.x) + 
				" h/2:" + std::to_string(col1HalfSizes.y) + 
				"], \n  col2: [x:" + std::to_string(col2Pos.x) + 
				" y:" + std::to_string(col2Pos.y) + 
				" w/2:" + std::to_string(col2HalfSizes.x) +
				" h/2:" + std::to_string(col2HalfSizes.y) + "]");
		}
		if (input->IsKeyDown(Input::INPUT_RIGHT))
		{
			//DebugLog::Info("Right key pressed.");
			col1Pos.x += 0.01f;
			col1->SetPosition(col1Pos);
			DebugLog::Info("\n  col1: [x:" + std::to_string(col1Pos.x) +
				" y:" + std::to_string(col1Pos.y) +
				" w/2:" + std::to_string(col1HalfSizes.x) +
				" h/2:" + std::to_string(col1HalfSizes.y) +
				"], \n  col2: [x:" + std::to_string(col2Pos.x) +
				" y:" + std::to_string(col2Pos.y) +
				" w/2:" + std::to_string(col2HalfSizes.x) +
				" h/2:" + std::to_string(col2HalfSizes.y) + "]");
		}
		if (input->IsMouseButtonDown(SDL_BUTTON_LEFT))
			DebugLog::Info("Left mouse button pressed.");
		if (input->IsMouseButtonDown(SDL_BUTTON_RIGHT))
			DebugLog::Info("Right mouse button pressed.");
		if (input->IsMouseButtonDown(SDL_BUTTON_MIDDLE))
			DebugLog::Info("Middle mouse button pressed.");

		engine->Update();

		physics->Update();

		engine->Draw();
	}

	std::cout << "End of engine life." << std::endl;

	// Delete the input manager.
	delete input;
	input = nullptr;

	// Delete the physics engine.
	delete physics;
	physics = nullptr;

	// destroy engine
	delete engine;
	engine = NULL;

	// Clean up SDL.
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}