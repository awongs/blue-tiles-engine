#define SDL_MAIN_HANDLED
#include <engine/GameWindow.h>
#include <engine/GameEngine.h>
#include <engine/input/Input.h>
#include "Level.h"



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

	// TODO: Collision-testing for physics engine, remove this later.
	//glm::vec3 col1HalfSizes{ 2.f };
	//Collider *col1{ new Collider(col1HalfSizes) };
	//glm::vec3 col1Pos{ 0.f };
	//col1->SetPosition(col1Pos);
	//physics->AddPhysicsObject(0, col1, 
	//	[](const PhysicsObject &other) { });

	//glm::vec3 col2HalfSizes{ 3.f };
	//Collider *col2{ new Collider(col2HalfSizes) };
	//glm::vec3 col2Pos{ 6.f, 0.f, 0.f };
	//col2->SetPosition(col2Pos);
	//physics->AddPhysicsObject(1, col2, 
	//	[](const PhysicsObject &other) { });

	// Create the input manager.
	Input* input{ new Input() };

	SDL_Event windowEvent;

	Level* l = new Level("level0");

	// Empty loop to prevent the window from closing immediately.
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;

			engine->HandleInput(input, windowEvent);

			// Quit the game with cancel key.
			if (input->IsKeyDown(Input::INPUT_CANCEL))
				break;
		}

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

	return 0;
}