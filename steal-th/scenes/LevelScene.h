#pragma once

#include "../blue-tiles-engine/engine/Scene.h"
#include "../Level.h"
#include "../gameobjects/Tile.h"

class PhysicsEngine;

class LevelScene : public Scene
{
public:
	LevelScene(Level* level, PhysicsEngine *physEngine);
	
	// The tiles in the level
	std::vector<Tile> tiles;
private:
	// Add a wall to the level
	void AddWall(std::string facing, int location, int width, int length);

	// Add a door to the level
	void AddDoor(std::string facing, std::string name, int location, int width, int length);

	// Current number of gameobjects in level
	int m_count;

	// Hold a raw pointer to the GameEngine's PhysicsEngine.
	// This will be needed for registering PhysicsObjects when PhysicsBehaviours
	// are added to GameObjects.
	PhysicsEngine *m_physEngine;
};