#define SDL_MAIN_HANDLED

#include <ctime>
#include <glm/gtc/random.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <memory>

#include <engine/GameWindow.h>
#include <engine/GameEngine.h>
#include <engine/Scene.h>
#include <engine/input/Input.h>
#include <engine/behaviours/MeshRenderer.h>
#include <engine/behaviours/DirectionalLight.h>
#include <engine/behaviours/PointLight.h>
#include <engine/behaviours/SpotLight.h>
#include <engine/physics/PhysicsObject.h>
#include <engine/physics/PhysicsEngine.h>
#include <engine/graphics/Camera.h>

#include "Level.h"
#include "scenes/LevelScene.h"

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
	PhysicsEngine *physEngine{ engine->GetPhysicsEngine() };

	// Create the level
	Level* l = new Level("level0");
	std::unique_ptr<LevelScene> level = std::make_unique<LevelScene>(l, physEngine);
	
	// -- Testing --
	srand(time(0));

	// Add a directional light
	GameObject* ga = new GameObject(420);
	ga->AddBehaviour(new DirectionalLight(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, -1.0f), 0.0f, 0.4f, 0.5f));
	level->AddWorldGameObject(ga);

	// Add point lights
	for (int i = 0; i < 10; i++)
	{
		GameObject* ga = new GameObject(999 + i);
		ga->position = level->GetWorldGameObjectByIndex(i)->position;

		glm::vec3 randomColour = glm::vec3(glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f));

		ga->AddBehaviour(new PointLight(randomColour, 1.0f, 1.0f, 1.0f, 0.2f, 0.3f, 0.5f));
		level->AddWorldGameObject(ga);
	}

	// Set the scene in engine
	engine->SetScene(level.release());

	SDL_Event windowEvent;

	// Empty loop to prevent the window from closing immediately.
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			Input::GetInstance().HandleInput(windowEvent);
			if (windowEvent.type == SDL_QUIT) break;

			// Quit the game with cancel key.
			if (Input::GetInstance().IsKeyDown(Input::INPUT_CANCEL))
				break;
		}

		engine->Update();

		engine->Draw();
	}

	std::cout << "End of engine life." << std::endl;

	// destroy engine
	delete engine;
	engine = NULL;

	return 0;
}