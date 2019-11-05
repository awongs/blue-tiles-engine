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
#include <engine/behaviours/TextBehaviour.h>

#include "Level.h"
#include "scenes/LevelScene.h"

// Window size settings.
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// Camera settings.
constexpr float CAMERA_FOV = glm::radians(60.0f);
constexpr float CAMERA_NEAR_CLIP = 1.0f;
constexpr float CAMERA_FAR_CLIP = 50.0f;
constexpr glm::vec3 CAMERA_ORIENTATION = glm::vec3(glm::radians(75.0f), 0.0f, 0.0f);

int main()
{
	GameWindow gameWin(WINDOW_WIDTH, WINDOW_HEIGHT);

	int windowSetupStatus = gameWin.SetupSDLWindow();

	if (windowSetupStatus)
	{
		return 1;
	}

	// create game engine
	GameEngine* engine = new GameEngine(gameWin.GetWindow());
	PhysicsEngine *physEngine{ engine->GetPhysicsEngine() };

	// Create the level
	Level* l = new Level("level1");
	std::unique_ptr<LevelScene> level = std::make_unique<LevelScene>(l, physEngine);

	// Setup the camera.
	Camera::GetInstance().SetOrientation(CAMERA_ORIENTATION);
	Camera::GetInstance().CalculatePerspectiveView(CAMERA_FOV, (float)WINDOW_WIDTH / WINDOW_HEIGHT, CAMERA_NEAR_CLIP, CAMERA_FAR_CLIP);
	
	// -- Testing --
	srand(time(0));

	// Add a directional light
	GameObject* ga = new GameObject();
	ga->AddBehaviour(new DirectionalLight(glm::vec3(0.6f), glm::vec3(0.0f, -10.0f, -0.3f), 0.0f, 0.4f, 0.5f));
	
	level->AddWorldGameObject(ga);
	
	// Test text
	GameObject* text = new GameObject();
	text->AddBehaviour(new TextBehaviour("OZMA", 2, glm::vec3(1, 0, 0)));
	level->AddScreenGameObject(text);

	// Add test lighting
	for (int i = 0; i < 0; i++)
	{
		GameObject* ga = new GameObject();
		ga->position = level->GetWorldGameObjectByIndex(i)->position;

		glm::vec3 randomColour = glm::vec3(glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f), glm::linearRand<float>(0.0f, 1.0f));
		//glm::vec3 randomDirection = glm::vec3(glm::linearRand<float>(-1.0f, 1.0f), glm::linearRand<float>(-1.0f, 1.0f), glm::linearRand<float>(-1.0f, 1.0f));

		ga->AddBehaviour(new PointLight(randomColour, 1.0f, 1.0f, 1.0f, 0.2f, 0.3f, 0.5f));
		//ga->AddBehaviour(new SpotLight(randomColour, randomDirection, 1.0, 2.0, 1.0f, 1.0f, 1.0f, 0.2f, 0.3f, 0.5f));
		
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