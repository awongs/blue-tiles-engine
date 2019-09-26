#pragma once

#include <list>
#include <memory>

#include "engine/GameObject.h"

class Scene
{
public:
	Scene(std::list<std::unique_ptr<GameObject>>& worldGameObjects, std::list<std::unique_ptr<GameObject>>& screenGameObjects);
	~Scene();

	// Updates game objects
	void Update();

	// Draws world game objects
	void DrawWorld();

	// Draws screen game objects
	void DrawScreen();
private:
	Scene() {};

	std::list<std::unique_ptr<GameObject>> m_worldGameObjects;
	std::list<std::unique_ptr<GameObject>> m_screenGameObjects;
};