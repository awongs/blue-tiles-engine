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
#include <engine/behaviours/UIMenuBehaviour.h>
#include <engine/behaviours/UIImageBehaviour.h>
#include <engine/behaviours/UIButtonBehaviour.h>
#include <engine/behaviours/UITextBehaviour.h>
#include <engine/behaviours/UILayoutBehaviour.h>
#include <util/FileManager.h>

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
	bool keepRunning = true;
	GameWindow gameWin(WINDOW_WIDTH, WINDOW_HEIGHT);

	int windowSetupStatus = gameWin.SetupSDLWindow();

	if (windowSetupStatus)
	{
		return 1;
	}

	// create game engine
	GameEngine* engine = new GameEngine(gameWin.GetWindow());
	PhysicsEngine *physEngine{ engine->GetPhysicsEngine() };

	// Create the main menu
	std::unique_ptr<Scene> mainMenuScene = std::make_unique<Scene>();

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
	ga->AddBehaviour(new DirectionalLight(glm::vec3(1.0f), glm::vec3(0.0f, -10.0f, -0.3f), 0.0f, 0.4f, 0.5f));
	
	level->AddWorldGameObject(ga);

	// UI for main menu
	GameObject* mainMenuBackgroundGO = new GameObject();
	mainMenuBackgroundGO->AddBehaviour(new UIMenuBehaviour("background", ImVec2(-10,-10), ImVec2(WINDOW_WIDTH + 30, WINDOW_HEIGHT + 30), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar));
	GameObject* mainMenuBackground = new GameObject();
	mainMenuBackground->AddBehaviour(new UIImageBehaviour("../Assets/textures/main_menu.png", ImVec2(WINDOW_WIDTH + 30, WINDOW_HEIGHT + 30)));
	mainMenuBackground->SetParent(mainMenuBackgroundGO);
	mainMenuScene->AddScreenGameObject(mainMenuBackgroundGO);
	GameObject* mainMenuGO = new GameObject();
	mainMenuGO->AddBehaviour(new UIMenuBehaviour("Steal-th", ImVec2((WINDOW_WIDTH / 2) - (WINDOW_WIDTH / 10), WINDOW_HEIGHT - 100), ImVec2((WINDOW_WIDTH / 4), 75), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground));
	GameObject* mainLayoutGO = new GameObject();
	mainLayoutGO->AddBehaviour(new UILayoutBehaviour(true));
	mainLayoutGO->SetParent(mainMenuGO);
	GameObject* playButtonGO = new GameObject();
	playButtonGO->AddBehaviour(new UIButtonBehaviour("Play", [&] {
		engine->SetScene(level.get());
		}, ImVec2(75, 50)));
	playButtonGO->SetParent(mainLayoutGO);
	GameObject* quitButtonGO = new GameObject();
	quitButtonGO->AddBehaviour(new UIButtonBehaviour("Quit", [&] {
		keepRunning = false;
		}, ImVec2(75, 50)));
	quitButtonGO->SetParent(mainLayoutGO);
	mainMenuScene->AddScreenGameObject(mainMenuGO);

	// UI for level
	GameObject* menu = new GameObject();
	menu->AddBehaviour(new UIMenuBehaviour("Inventory", ImVec2(0, 0), ImVec2(0, 0), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground));
	GameObject* testString = new GameObject();
	testString->AddBehaviour(new UITextBehaviour("Inventory"));
	testString->SetParent(menu);
	GameObject* redKey = new GameObject("redKey");
	redKey->isVisible = false;
	redKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/red_key_block.png"));
	redKey->SetParent(menu);
	GameObject* greenKey = new GameObject("greenKey");
	greenKey->isVisible = false;
	greenKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/green_key_block.png"));
	greenKey->SetParent(menu);
	GameObject* blueKey = new GameObject("blueKey");
	blueKey->isVisible = false;
	blueKey->AddBehaviour(new UIImageBehaviour("../Assets/textures/blue_key_block.png"));
	blueKey->SetParent(menu);
	level->AddScreenGameObject(menu);

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
	engine->SetScene(mainMenuScene.get());

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

	level.release();
	mainMenuScene.release();

	// destroy engine
	delete engine;
	engine = NULL;

	return 0;
}