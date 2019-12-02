#pragma once

#include <memory>
#include <engine/Scene.h>

class GameEngine;

class MainMenuScene : public Scene
{
public:
	MainMenuScene(std::shared_ptr<bool> keepRunning);

private:
	std::shared_ptr<bool> m_keepRunning;
};