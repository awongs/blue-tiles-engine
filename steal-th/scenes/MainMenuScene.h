#pragma once

#include <memory>
#include <engine/Scene.h>

class GameEngine;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(std::shared_ptr<bool> keepRunning, std::shared_ptr<GameEngine> gameEngine);

private:
	std::shared_ptr<bool> m_keepRunning;
	std::shared_ptr<GameEngine> m_gameEngine;
};