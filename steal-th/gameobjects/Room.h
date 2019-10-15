#pragma once

#include "Door.h"
#include "Wall.h"
#include <vector>

struct Room
{
	int roomid;
	std::vector<int> gridUsed;
	std::vector<Door> doors;
	std::vector<Wall> walls;
};