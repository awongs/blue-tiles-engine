#define SDL_MAIN_HANDLED

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
#include <engine/behaviours/UIMenuBehaviour.h>
#include <engine/behaviours/UIImageBehaviour.h>
#include <engine/behaviours/UIButtonBehaviour.h>
#include <engine/behaviours/UITextBehaviour.h>
#include <engine/behaviours/UILayoutBehaviour.h>
#include <util/FileManager.h>

#include "Level.h"
#include "scenes/LevelScene.h"
#include "scenes/MainMenuScene.h"

// Window size settings.
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;


int main()
{
	bool keepRunning = true;
	GameWindow gameWin(WINDOW_WIDTH, WINDOW_HEIGHT);

	int windowSetupStatus = gameWin.SetupSDLWindow();

	if (windowSetupStatus)
	{
		return 1;
	}

	// create game engine
	std::shared_ptr<GameEngine> engine = std::make_shared<GameEngine>(gameWin.GetWindow());
	PhysicsEngine *physEngine{ engine->GetPhysicsEngine() };

	// Create the main menu
	std::shared_ptr<MainMenuScene> mainMenuScene = std::make_shared<MainMenuScene>(keepRunning, engine);
	engine->AddScene("mainMenu", mainMenuScene, [&] {
		mainMenuScene = std::make_shared<MainMenuScene>(keepRunning, engine);
		});

	// Create the level
	Level* l = new Level("level2");
	std::shared_ptr<LevelScene> level = std::make_shared<LevelScene>(l, physEngine, engine);
	engine->AddScene("level2", level, [&] {
		level = std::make_shared<LevelScene>(l, physEngine, engine);
		});

	// Set the scene in engine
	engine->SetScene("mainMenu");

	SDL_Event windowEvent;

	// Empty loop to prevent the window from closing immediately.
	while (keepRunning)
	{
		engine->HandleEvent(windowEvent);

		if (windowEvent.type == SDL_QUIT) break;

		// Quit the game with cancel key.
		if (Input::GetInstance().IsKeyDown(Input::INPUT_CANCEL)) break;

		engine->Update();

		engine->Draw();
	}

	std::cout << "End of engine life." << std::endl;

	return 0;
}