#pragma once


#include "../blue-tiles-engine/engine/Scene.h"
#include "../Level.h"

class LevelScene : public Scene
{
public:
	LevelScene(Level* level);
private:
	void AddWall(std::string facing, int location, int width, int length);

	int m_count;
};