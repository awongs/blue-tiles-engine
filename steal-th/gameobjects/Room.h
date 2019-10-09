#pragma once
#include "Door.h"
#include "Wall.h"
#include <vector>

class Room
{
	public:
		Room(int _roomid, std::vector<int> _gridUsed, std::vector<Door> _doors, std::vector<Wall> walls);

		int roomid;
		std::vector<int> gridUsed;
		std::vector<Door> doors;
		std::vector<Wall> walls;
};