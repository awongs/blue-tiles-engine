#include "MainMenuScene.h"

#include <engine/GameEngine.h>
#include <engine/behaviours/UIMenuBehaviour.h>
#include <engine/behaviours/UIImageBehaviour.h>
#include <engine/behaviours/UILayoutBehaviour.h>
#include <engine/behaviours/UIButtonBehaviour.h>

namespace
{
	// Window size settings.
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;
	const std::string levelName = "level1";
}

MainMenuScene::MainMenuScene(bool& keepRunning, std::shared_ptr<GameEngine> gameEngine)
	: Scene()
	, m_keepRunning(&keepRunning)
	, m_gameEngine(gameEngine)
{
	// UI for main menu
	GameObject* mainMenuBackgroundGO = new GameObject();
	mainMenuBackgroundGO->AddBehaviour(new UIMenuBehaviour("background", ImVec2(-10, -10), ImVec2(WINDOW_WIDTH + 30, WINDOW_HEIGHT + 30), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar));
	AddScreenGameObject(mainMenuBackgroundGO);

	GameObject* mainMenuBackground = new GameObject();
	mainMenuBackground->AddBehaviour(new UIImageBehaviour("../Assets/textures/main_menu.png", ImVec2(WINDOW_WIDTH + 30, WINDOW_HEIGHT + 30)));
	mainMenuBackground->SetParent(mainMenuBackgroundGO);
	AddScreenGameObject(mainMenuBackground);

	GameObject* mainMenuGO = new GameObject();
	mainMenuGO->AddBehaviour(new UIMenuBehaviour("Steal-th", ImVec2((WINDOW_WIDTH / 2) - (WINDOW_WIDTH / 10), WINDOW_HEIGHT - 100), ImVec2((WINDOW_WIDTH / 4), 75), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground));
	AddScreenGameObject(mainMenuGO);

	GameObject* mainLayoutGO = new GameObject();
	mainLayoutGO->AddBehaviour(new UILayoutBehaviour(true));
	mainLayoutGO->SetParent(mainMenuGO);
	AddScreenGameObject(mainLayoutGO);

	GameObject* playButtonGO = new GameObject();
	playButtonGO->AddBehaviour(new UIButtonBehaviour("Play", [&] {
		m_gameEngine->SetScene("level2");
		}, ImVec2(75, 50)));
	playButtonGO->SetParent(mainLayoutGO);
	AddScreenGameObject(playButtonGO);

	GameObject* quitButtonGO = new GameObject();
	quitButtonGO->AddBehaviour(new UIButtonBehaviour("Quit", [&] {
		(*m_keepRunning) = !(*m_keepRunning);
		}, ImVec2(75, 50)));
	quitButtonGO->SetParent(mainLayoutGO);
	AddScreenGameObject(quitButtonGO);
}