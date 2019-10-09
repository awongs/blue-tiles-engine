#define SDL_MAIN_HANDLED
#include <ctime>
#include <glm/gtc/random.hpp>
#include <glm/ext/matrix_transform.hpp>

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

	// -- Testing --
	srand(time(0));
	std::vector<std::unique_ptr<GameObject>> worldGameObjects;
	std::vector<std::unique_ptr<GameObject>> screenGameObjects;
	for (int i = 0; i < 256; i++)
	{
		MeshRenderer* meshRenderer = new MeshRenderer("../Assets/models/cube.obj");
		meshRenderer->SetTexture("../Assets/textures/golden_goose.png");

		// Add the physics component to the game object.
		Collider* col{ new Collider(glm::vec3(2.f)) };

		PhysicsObject* physObj = new PhysicsObject(col, [](const PhysicsObject& other) {});

		// Register this physics component to the physics engine.
		//m_physEngine->AddPhysicsObject(physObj);

		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(i);
		ga->AddBehaviour(meshRenderer);
		ga->AddBehaviour(physObj);

		ga->position += glm::vec3(glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f));

		// Update the collider's position.
		col->SetPosition(ga->position);

		worldGameObjects.push_back(std::move(ga));
	}

	// Add a directional light
	std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(420);
	ga->AddBehaviour(new DirectionalLight(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, -1.0f), 0.0f, 0.3f, 0.5f));

	// Add point lights
	for (int i = 0; i < 64; i++)
	{
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(999 + i);
		ga->position += glm::vec3(glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f));

		glm::vec3 randomColour = glm::vec3(glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f));

		ga->AddBehaviour(new PointLight(randomColour));
		worldGameObjects.push_back(std::move(ga));
	}

	// Add spot lights
	for (int i = 0; i < 32; i++)
	{
		std::unique_ptr<GameObject> ga = std::make_unique<GameObject>(1628 + i);
		ga->position += glm::vec3(glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f), glm::linearRand<float>(-10.0f, 10.0f));

		glm::vec3 randomColour = glm::vec3(glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f));
		glm::vec3 randomDirection = glm::normalize(glm::vec3(glm::linearRand<float>(-1.0f, 1.0f), glm::linearRand<float>(-1.0f, 1.0f), glm::linearRand<float>(-1.0f, 1.0f)));

		ga->AddBehaviour(new SpotLight(randomColour, randomDirection, 2, 2.5));
		worldGameObjects.push_back(std::move(ga));
	}

	worldGameObjects.push_back(std::move(ga));

	engine->SetScene(new Scene(worldGameObjects, screenGameObjects));

	DebugLog::Info("Test scene initialization completed!");

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