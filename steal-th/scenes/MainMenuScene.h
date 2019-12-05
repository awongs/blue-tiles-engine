#pragma once

#include <memory>
#include <engine/Scene.h>

class GameEngine;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(bool& keepRunning, std::shared_ptr<GameEngine> gameEngine);

private:
	bool* m_keepRunning;
	std::shared_ptr<GameEngine> m_gameEngine;
};