#pragma once


#include "../blue-tiles-engine/engine/Scene.h"
#include "../Level.h"
#include "../gameobjects/Tile.h"

class LevelScene : public Scene
{
public:
	LevelScene(Level* _level);

	// The tiles in the level
	std::vector<Tile> tiles;

	// Level object
	Level* level;

private:
	// Add a wall to the level
	void AddWall(std::string facing, int location, int width, int length);

	// Add a door to the level
	void AddDoor(std::string facing, std::string name, int location, int width, int length);

	// Current number of gameobjects in level
	int m_count;
};